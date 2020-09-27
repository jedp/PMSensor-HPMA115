#include "HPMA115_Compact.h"
#include "aqi.h"

HPMA115_Compact::HPMA115_Compact()
{
}

bool HPMA115_Compact::begin(Stream *stream, compact_auto_result_t *data) {
  hpma = stream;
  result_data = data;

  return true;
}

compact_result_error_t HPMA115_Compact::checkAutoReceive() {
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
  uint8_t buf[30] = {};
  hpma->readBytes(buf, 30);

  result_data->pm1  = (buf[DATA0_H] << 8) + buf[DATA0_L];
  result_data->pm25 = (buf[DATA1_H] << 8) + buf[DATA1_L];
  result_data->pm4  = (buf[DATA2_H] << 8) + buf[DATA2_L];
  result_data->pm10 = (buf[DATA3_H] << 8) + buf[DATA3_L];

  uint32_t aqi25 = aqi_pm25(result_data->pm25);
  uint32_t aqi10 = aqi_pm10(result_data->pm10);

  result_data->aqi = (aqi25 > aqi10) ? aqi25 : aqi10;

  return NEW_DATA;
}

