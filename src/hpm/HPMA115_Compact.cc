#include <stdio.h>
#include <string.h>
#include "hpm/HPMA115_Compact.h"
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

compact_auto_status_t HPMA115_Compact::checkAutoReceive() {
  if (!hpma) {
    return DEVICE_UNAVAILABLE;
  }

  if (hpma->available() < 1) {
    return NO_DATA;
  }

  if (hpma->read() != AUTO_HEAD_0) {
    // Not at the head of a protocol block. Skip to next byte.
    return NOT_HEAD0_RETRY;
  }

  if (hpma->read() != AUTO_HEAD_1) {
    // That last byte was not in fact the start of the header.
    return NOT_HEAD1_RETRY;
  }

  if (hpma->available() < 30) {
    return INSUFFICIENT_DATA;
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
    return BAD_CHECKSUM;
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

  return NEW_DATA;
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

  printf("for cmd %02X cs is %02X\n", cmdByte, cmd[3]);

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

