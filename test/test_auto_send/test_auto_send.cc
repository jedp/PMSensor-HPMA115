#include <unity.h>

#include "aqi/aqi.h"
#include "hpm/HPMA115_Compact.h"
#include "fake/FakeStream.h"

void test_stream_unavailable(void) {
  HPMA115_Compact hpm = HPMA115_Compact();

  TEST_ASSERT_EQUAL(DEVICE_UNAVAILABLE, hpm.checkAutoReceive());
}

void test_stream_has_no_data(void) {
  HPMA115_Compact hpm = HPMA115_Compact();
  Stream fakeStream;
  hpm.begin(&fakeStream);

  TEST_ASSERT_EQUAL(NO_DATA, hpm.checkAutoReceive());
}

void test_retry_to_read_head0(void) {
  HPMA115_Compact hpm = HPMA115_Compact();
  Stream fakeStream;
  hpm.begin(&fakeStream);

  fakeStream.add(0x01);
  TEST_ASSERT_EQUAL(NOT_HEAD0_RETRY, hpm.checkAutoReceive());
}

void test_retry_to_read_head1(void) {
  HPMA115_Compact hpm = HPMA115_Compact();
  Stream fakeStream;
  hpm.begin(&fakeStream);

  fakeStream.add(0x42);  // The correct HEAD0 byte.
  fakeStream.add(0x01);  // Not the HEAD1 byte.
  TEST_ASSERT_EQUAL(NOT_HEAD1_RETRY, hpm.checkAutoReceive());
}

void test_insufficient_data() {
  HPMA115_Compact hpm = HPMA115_Compact();
  Stream fakeStream;
  hpm.begin(&fakeStream);

  fakeStream.add(0x42);  // The correct HEAD0 byte.
  fakeStream.add(0x4D);  // the correct HEAD1 byte.
  fakeStream.add(0x01);  // Not enough data bytes.
  TEST_ASSERT_EQUAL(INSUFFICIENT_DATA, hpm.checkAutoReceive());
}

void test_bad_checksum(void) {
  HPMA115_Compact hpm = HPMA115_Compact();
  Stream fakeStream;
  hpm.begin(&fakeStream);

  fakeStream.add(0x42);  // The correct HEAD0 byte.
  fakeStream.add(0x4D);  // the correct HEAD1 byte.
  fakeStream.add(0x02);
  fakeStream.add(0x03);
  fakeStream.add(0x04);
  fakeStream.add(0x05);
  fakeStream.add(0x06);
  fakeStream.add(0x07);

  fakeStream.add(0x08);
  fakeStream.add(0x09);
  fakeStream.add(0x0A);
  fakeStream.add(0x0B);
  fakeStream.add(0x0C);
  fakeStream.add(0x0D);
  fakeStream.add(0x0E);
  fakeStream.add(0x0F);

  fakeStream.add(0x10);
  fakeStream.add(0x11);
  fakeStream.add(0x12);
  fakeStream.add(0x13);
  fakeStream.add(0x14);
  fakeStream.add(0x15);
  fakeStream.add(0x16);
  fakeStream.add(0x17);

  fakeStream.add(0x18);
  fakeStream.add(0x19);
  fakeStream.add(0x1A);
  fakeStream.add(0x1B);
  fakeStream.add(0x1C);
  fakeStream.add(0x1D);
  fakeStream.add(0xFF);  // Bogus high checksum byte.
  fakeStream.add(0xFF);  // Bogus low checksum byte.

  TEST_ASSERT_EQUAL(BAD_CHECKSUM, hpm.checkAutoReceive());
}

void test_valid_checksum(void) {
  HPMA115_Compact hpm = HPMA115_Compact();
  Stream fakeStream;
  hpm.begin(&fakeStream);

  fakeStream.add(0x42);  // The correct HEAD0 byte.
  fakeStream.add(0x4D);  // the correct HEAD1 byte.
  fakeStream.add(0x02);  // Bogus high length byte.
  fakeStream.add(0x03);  // Bogus low length byte.
  fakeStream.add(0x04);  // PM1 High
  fakeStream.add(0x05);  // PM1 Low
  fakeStream.add(0x06);  // PM25 High
  fakeStream.add(0x07);  // PM25 Low

  fakeStream.add(0x08);  // PM4 High
  fakeStream.add(0x09);  // PM4 Low
  fakeStream.add(0x0A);  // PM10 High
  fakeStream.add(0x0B);  // PM10 Low
  fakeStream.add(0x0C);  // Reserved ...
  fakeStream.add(0x0D);
  fakeStream.add(0x0E);
  fakeStream.add(0x0F);

  fakeStream.add(0x10);
  fakeStream.add(0x11);
  fakeStream.add(0x12);
  fakeStream.add(0x13);
  fakeStream.add(0x14);
  fakeStream.add(0x15);
  fakeStream.add(0x16);
  fakeStream.add(0x17);

  fakeStream.add(0x18);
  fakeStream.add(0x19);
  fakeStream.add(0x1A);
  fakeStream.add(0x1B);
  fakeStream.add(0x1C);
  fakeStream.add(0x1D);
  fakeStream.add(0x02);  // Valid checksum high.
  fakeStream.add(0x41);  // Valid checksum low.

  TEST_ASSERT_EQUAL(NEW_DATA, hpm.checkAutoReceive());
}

void test_valid_data(void) {
  HPMA115_Compact hpm = HPMA115_Compact();
  Stream fakeStream;
  hpm.begin(&fakeStream);

  fakeStream.add(0x42);  // The correct HEAD0 byte.
  fakeStream.add(0x4D);  // the correct HEAD1 byte.
  fakeStream.add(0x02);  // Bogus high length byte.
  fakeStream.add(0x03);  // Bogus low length byte.
  fakeStream.add(0x04);  // PM1 High
  fakeStream.add(0x05);  // PM1 Low
  fakeStream.add(0x06);  // PM25 High
  fakeStream.add(0x07);  // PM25 Low

  fakeStream.add(0x08);  // PM4 High
  fakeStream.add(0x09);  // PM4 Low
  fakeStream.add(0x0A);  // PM10 High
  fakeStream.add(0x0B);  // PM10 Low
  fakeStream.add(0x0C);  // Reserved ...
  fakeStream.add(0x0D);
  fakeStream.add(0x0E);
  fakeStream.add(0x0F);

  fakeStream.add(0x10);
  fakeStream.add(0x11);
  fakeStream.add(0x12);
  fakeStream.add(0x13);
  fakeStream.add(0x14);
  fakeStream.add(0x15);
  fakeStream.add(0x16);
  fakeStream.add(0x17);

  fakeStream.add(0x18);
  fakeStream.add(0x19);
  fakeStream.add(0x1A);
  fakeStream.add(0x1B);
  fakeStream.add(0x1C);
  fakeStream.add(0x1D);
  fakeStream.add(0x02);  // Valid checksum high.
  fakeStream.add(0x41);  // Valid checksum low.

  TEST_ASSERT_EQUAL(NEW_DATA, hpm.checkAutoReceive());
  TEST_ASSERT_EQUAL(static_cast<uint16_t> (0x04 << 8) + 0x05, hpm.getPM1());
  TEST_ASSERT_EQUAL(static_cast<uint16_t> (0x06 << 8) + 0x07, hpm.getPM25());
  TEST_ASSERT_EQUAL(static_cast<uint16_t> (0x08 << 8) + 0x09, hpm.getPM4());
  TEST_ASSERT_EQUAL(static_cast<uint16_t> (0x0A << 8) + 0x0B, hpm.getPM10());
}

void test_aqi_when_pm25_higher(void) {
  HPMA115_Compact hpm = HPMA115_Compact();
  Stream fakeStream;
  hpm.begin(&fakeStream);

  fakeStream.add(0x42);  // The correct HEAD0 byte.
  fakeStream.add(0x4D);  // the correct HEAD1 byte.
  fakeStream.add(0x02);  // Bogus high length byte.
  fakeStream.add(0x03);  // Bogus low length byte.
  fakeStream.add(0x04);  // PM1 High
  fakeStream.add(0x05);  // PM1 Low
  fakeStream.add(0x06);  // PM25 High (Higher AQI than PM10 gives)
  fakeStream.add(0x07);  // PM25 Low

  fakeStream.add(0x08);  // PM4 High
  fakeStream.add(0x09);  // PM4 Low
  fakeStream.add(0x00);  // PM10 High
  fakeStream.add(0x01);  // PM10 Low (Lower AQI than PM2.5 gives);
  fakeStream.add(0x0C);  // Reserved ...
  fakeStream.add(0x0D);
  fakeStream.add(0x0E);
  fakeStream.add(0x0F);

  fakeStream.add(0x10);
  fakeStream.add(0x11);
  fakeStream.add(0x12);
  fakeStream.add(0x13);
  fakeStream.add(0x14);
  fakeStream.add(0x15);
  fakeStream.add(0x16);
  fakeStream.add(0x17);

  fakeStream.add(0x18);
  fakeStream.add(0x19);
  fakeStream.add(0x1A);
  fakeStream.add(0x1B);
  fakeStream.add(0x1C);
  fakeStream.add(0x1D);
  fakeStream.add(0x02);  // Valid checksum high.
  fakeStream.add(0x2D);  // Valid checksum low.

  TEST_ASSERT_EQUAL(NEW_DATA, hpm.checkAutoReceive());
  TEST_ASSERT_EQUAL(
      aqi_pm25(static_cast<uint16_t> (0x06 << 8) + 0x07),
      hpm.getAQI());
}

void test_aqi_when_pm10_higher(void) {
  HPMA115_Compact hpm = HPMA115_Compact();
  Stream fakeStream;
  hpm.begin(&fakeStream);

  fakeStream.add(0x42);  // The correct HEAD0 byte.
  fakeStream.add(0x4D);  // the correct HEAD1 byte.
  fakeStream.add(0x02);  // Bogus high length byte.
  fakeStream.add(0x03);  // Bogus low length byte.
  fakeStream.add(0x04);  // PM1 High
  fakeStream.add(0x05);  // PM1 Low
  fakeStream.add(0x00);  // PM25 High
  fakeStream.add(0x01);  // PM25 Low (Lower AQI than PM10 gives)

  fakeStream.add(0x08);  // PM4 High
  fakeStream.add(0x09);  // PM4 Low
  fakeStream.add(0x01);  // PM10 High (Higher AQI than PM2.5 gives)
  fakeStream.add(0x0B);  // PM10 Low
  fakeStream.add(0x0C);  // Reserved ...
  fakeStream.add(0x0D);
  fakeStream.add(0x0E);
  fakeStream.add(0x0F);

  fakeStream.add(0x10);
  fakeStream.add(0x11);
  fakeStream.add(0x12);
  fakeStream.add(0x13);
  fakeStream.add(0x14);
  fakeStream.add(0x15);
  fakeStream.add(0x16);
  fakeStream.add(0x17);

  fakeStream.add(0x18);
  fakeStream.add(0x19);
  fakeStream.add(0x1A);
  fakeStream.add(0x1B);
  fakeStream.add(0x1C);
  fakeStream.add(0x1D);
  fakeStream.add(0x02);  // Valid checksum high.
  fakeStream.add(0x2C);  // Valid checksum low.

  TEST_ASSERT_EQUAL(NEW_DATA, hpm.checkAutoReceive());
  TEST_ASSERT_EQUAL(
      aqi_pm10(static_cast<uint16_t> (0x01 << 8) + 0x0B),
      hpm.getAQI());
}

int main(int argc, char** argv) {
  UNITY_BEGIN();

  test_stream_unavailable();
  test_stream_has_no_data();
  test_retry_to_read_head0();
  test_retry_to_read_head1();
  test_bad_checksum();
  test_valid_checksum();
  test_valid_data();
  test_aqi_when_pm25_higher();
  test_aqi_when_pm10_higher();

  UNITY_END();
}

