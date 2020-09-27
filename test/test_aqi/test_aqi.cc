#include <unity.h>

#include "aqi/aqi.h"

void test_pm25_aqi(void) {
  // "Good"
  TEST_ASSERT_EQUAL(0, aqi_pm25(0));
  TEST_ASSERT_EQUAL(25, aqi_pm25(6));
  TEST_ASSERT_EQUAL(50, aqi_pm25(12));

  // "Moderate"
  TEST_ASSERT_EQUAL(51, aqi_pm25(13));
  TEST_ASSERT_EQUAL(75, aqi_pm25(24));
  TEST_ASSERT_EQUAL(100, aqi_pm25(35));

  // "Unhealthy for sensitive groups"
  TEST_ASSERT_EQUAL(101, aqi_pm25(36));
  TEST_ASSERT_EQUAL(124, aqi_pm25(45));
  TEST_ASSERT_EQUAL(150, aqi_pm25(55));

  // "Unhealthy"
  TEST_ASSERT_EQUAL(151, aqi_pm25(56));
  TEST_ASSERT_EQUAL(175, aqi_pm25(103));
  TEST_ASSERT_EQUAL(200, aqi_pm25(150));

  // "Very unhealthy"
  TEST_ASSERT_EQUAL(201, aqi_pm25(151));
  TEST_ASSERT_EQUAL(250, aqi_pm25(200));
  TEST_ASSERT_EQUAL(300, aqi_pm25(250));

  // "Hazardous"
  TEST_ASSERT_EQUAL(301, aqi_pm25(251));
  TEST_ASSERT_EQUAL(350, aqi_pm25(300));
  TEST_ASSERT_EQUAL(400, aqi_pm25(350));

  // "Extra specially hazardous"
  TEST_ASSERT_EQUAL(401, aqi_pm25(351));
  TEST_ASSERT_EQUAL(450, aqi_pm25(425));
  TEST_ASSERT_EQUAL(500, aqi_pm25(500));
}

void test_pm10_aqi(void) {
  // "Good"
  TEST_ASSERT_EQUAL(0, aqi_pm10(0));
  TEST_ASSERT_EQUAL(25, aqi_pm10(27));
  TEST_ASSERT_EQUAL(50, aqi_pm10(54));

  // "Moderate"
  TEST_ASSERT_EQUAL(51, aqi_pm10(55));
  TEST_ASSERT_EQUAL(75, aqi_pm10(104));
  TEST_ASSERT_EQUAL(100, aqi_pm10(154));

  // "Unhealthy for sensitive groups"
  TEST_ASSERT_EQUAL(101, aqi_pm10(155));
  TEST_ASSERT_EQUAL(125, aqi_pm10(204));
  TEST_ASSERT_EQUAL(150, aqi_pm10(254));

  // "Unhealthy"
  TEST_ASSERT_EQUAL(151, aqi_pm10(255));
  TEST_ASSERT_EQUAL(175, aqi_pm10(304));
  TEST_ASSERT_EQUAL(200, aqi_pm10(354));

  // "Very unhealthy"
  TEST_ASSERT_EQUAL(201, aqi_pm10(355));
  TEST_ASSERT_EQUAL(251, aqi_pm10(390));
  TEST_ASSERT_EQUAL(300, aqi_pm10(424));

  // "Hazardous"
  TEST_ASSERT_EQUAL(301, aqi_pm10(425));
  TEST_ASSERT_EQUAL(351, aqi_pm10(465));
  TEST_ASSERT_EQUAL(400, aqi_pm10(504));

  // "Extra specially hazardous"
  TEST_ASSERT_EQUAL(401, aqi_pm10(505));
  TEST_ASSERT_EQUAL(450, aqi_pm10(554));
  TEST_ASSERT_EQUAL(500, aqi_pm10(604));
}

int main(int argc, char** argv) {
  UNITY_BEGIN();

  test_pm25_aqi();
  test_pm10_aqi();

  UNITY_END();
}

