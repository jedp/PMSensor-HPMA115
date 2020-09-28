#include <unity.h>

#include "aqi/aqi.h"
#include "hpm/HPMA115_Compact.h"
#include "fake/FakeStream.h"

void test_read_measurement() {
  HPMA115_Compact hpm = HPMA115_Compact();
  Stream fakeStream;
  hpm.begin(&fakeStream);

  TEST_ASSERT_TRUE(hpm.readParticleMeasurementResults());

  // Receive example results from data sheet, Table 6.
  TEST_ASSERT_EQUAL(0x30, hpm.getPM1());
  TEST_ASSERT_EQUAL(0x31, hpm.getPM25());
  TEST_ASSERT_EQUAL(0x32, hpm.getPM4());
  TEST_ASSERT_EQUAL(0x33, hpm.getPM10());
  TEST_ASSERT_EQUAL(0x86, hpm.getAQI());
}

int main(int argc, char** argv) {
  UNITY_BEGIN();

  test_read_measurement();

  UNITY_END();
}
