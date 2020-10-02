#include <stdio.h>
#include "HPMA115_Compact.h"
#include "aqi/aqi.h"

HPMA115_Compact::HPMA115_Compact() {
}

bool HPMA115_Compact::begin(Stream *stream) {
  hpma = stream;

  return true;
}

uint16_t HPMA115_Compact::getPM1() {
  return result_data.pm1;
}

uint16_t HPMA115_Compact::getPM25() {
  return result_data.pm25;
}

uint16_t HPMA115_Compact::getPM4() {
  return result_data.pm4;
}

uint16_t HPMA115_Compact::getPM10() {
  return result_data.pm10;
}

uint16_t HPMA115_Compact::getAQI() {
  return result_data.aqi;
}

bool HPMA115_Compact::isNewDataAvailable() {
  if (!hpma) {
    return false;
  }

  if (hpma->available() < 1) {
    return false;
  }

  // Read ahead as much as possible, stopping at the header byte.
  bool found_head = false;
  for (uint8_t i = 0; i < hpma->available(); ++i) {
    if (hpma->read() == AUTO_HEAD_0) {
      found_head = true;
      break;
    }
  }

  if (!found_head) {
    return false;
  }

  if (hpma->read() != AUTO_HEAD_1) {
    // That last byte was not in fact the start of the header.
    return false;
  }

  if (hpma->available() < 30) {
    return false;
  }

  // We've already consumed the header bytes, so 30 remain in the message.
  uint8_t buf[AUTO_SEND_DATA_BYTES] = {};
  hpma->readBytes(buf, AUTO_SEND_DATA_BYTES);

  // Compute the checksum. This *does* include the header bytes, so start
  // by adding those.
  uint32_t expected_sum = (buf[CS_H] << 8) + buf[CS_L];
  uint32_t actual_sum = AUTO_HEAD_0 + AUTO_HEAD_1;
  for (uint8_t i = 0; i < CS_H; ++i) {
    actual_sum += buf[i];
  }

  if (actual_sum != expected_sum) {
    return false;
  }

  // Everything's great. We can report the readings.
  result_data.pm1  = (buf[DATA0_H] << 8) + buf[DATA0_L];
  result_data.pm25 = (buf[DATA1_H] << 8) + buf[DATA1_L];
  result_data.pm4  = (buf[DATA2_H] << 8) + buf[DATA2_L];
  result_data.pm10 = (buf[DATA3_H] << 8) + buf[DATA3_L];

  // As a bonus, we an do the crazy AQI conversion and report that, too.
  uint32_t aqi25 = aqi_pm25(result_data.pm25);
  uint32_t aqi10 = aqi_pm10(result_data.pm10);

  // The worse pollutant determines the AQI. Because the EPA said so.
  result_data.aqi = (aqi25 > aqi10) ? aqi25 : aqi10;

  return true;
}

bool HPMA115_Compact::readParticleMeasurementResults() {
  // Data sheet, Table 6.
  uint8_t cmd[] = { 0x68, 0x01, 0x04, 0x93 };
  hpma->write(cmd, 4);

  // Block until we get a response.
  // A successful response will contain the readings.
  while (hpma->available() < 1) {}
  uint8_t resp_head = hpma->read();
  if (resp_head != 0x40) {
    // Failure. Next byte should be 0x96. Swallow it and die.
    hpma->read();
    return false;
  }

  while (hpma->available() < 1) {}
  if (hpma->read() != 0x0D) {
    // Unexpected length.
    // Note that there is a typo in the datasheet here. The expected value
    // is 0x0D, not 0x05 as the data sheet says.
    return false;
  }

  while (hpma->available() < 1) {}
  if (hpma->read() != 0x04) {
    // Unexpected command
    return false;
  }

  // Read the 12 data bytes. The first 8 fields contain the PM readings.
  // The remaining 4 are reserved. We use them to calculate th checksum,
  // but otherwise ignore them.
  while (hpma->available() < 12) {}
  uint8_t buf[12] = { 0 };
  hpma->readBytes(buf, 12);

  while (hpma->available() < 1) {}
  uint32_t expected_sum = hpma->read();

  // Calculate the observed checksum.
  // First subtract the headers ...
  uint32_t actual_sum = 0x10000 - 0x40 - 0x0D - 0x04;
  // ... then the rest of the bytes.
  for (uint8_t i = 0; i < 12; ++i) {
    actual_sum -= buf[i];
  }
  actual_sum %= 256;

  if (actual_sum != expected_sum) {
    return false;
  }

  // Everything's great. We can report the readings.
  result_data.pm1  = (buf[0] << 8) + buf[1];
  result_data.pm25 = (buf[2] << 8) + buf[3];
  result_data.pm4  = (buf[4] << 8) + buf[5];
  result_data.pm10 = (buf[6] << 8) + buf[7];

  // As a bonus, we an do the crazy AQI conversion and report that, too.
  uint32_t aqi25 = aqi_pm25(result_data.pm25);
  uint32_t aqi10 = aqi_pm10(result_data.pm10);

  // The worse pollutant determines the AQI. Because the EPA said so.
  result_data.aqi = (aqi25 > aqi10) ? aqi25 : aqi10;

  return true;
}

bool HPMA115_Compact::stopParticleMeasurement() {
  // Data sheet, Table 6.
  return writeSimpleCommand(0x02);
}

bool HPMA115_Compact::startParticleMeasurement() {
  // Data sheet, Table 6.
  return writeSimpleCommand(0x01);
}

bool HPMA115_Compact::stopAutoSend() {
  // Data sheet, Table 6.
  return writeSimpleCommand(0x20);
}

bool HPMA115_Compact::startAutoSend() {
  // Data sheet, Table 6.
  return writeSimpleCommand(0x40);
}

bool HPMA115_Compact::writeSimpleCommand(uint8_t cmdByte) {
  uint8_t cmd[4];

  // Data sheet, Table 6.
  cmd[0] = 0x68;
  cmd[1] = 0x01;  // Length is guaranteed to be one.
  cmd[2] = cmdByte;
  cmd[3] = (0x10000 - (cmd[0] + cmd[1] + cmd[2]) % 256);

  hpma->write(cmd, 4);

  // Block until we get a response.
  while (hpma->available() < 2) {}
  uint8_t resp[2];
  hpma->readBytes(resp, 2);

  if (resp[0] == 0xA5 && resp[1] == 0xA5) {
    return true;
  }

  return false;
}

