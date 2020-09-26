#include "aqi.h"

/* Lower and upper bounds for AQI categories. */
#define GOOD_LO 0
#define GOOD_HI 50
#define MODT_LO 51
#define MODT_HI 100
#define UFSG_LO 101
#define UFSG_HI 150
#define UNHY_LO 151
#define UNHY_HI 200
#define VUNH_LO 201
#define VUNH_HI 300
#define HAZ1_LO 301
#define HAZ1_HI 400
#define HAZ2_LO 401
#define HAZ2_HI 500

/*
 * Internal function for computing AQI given concentration of pollutant,
 * index high and low boundaires, and pollutant bracket low and high.
 *
 * The magical values for the ranges are available on the EPA website,
 * but are probably much easier to find on Wikipedia.
 */
uint32_t _aqi(
    uint16_t c_p,
    uint16_t i_hi,
    uint16_t i_lo,
    uint16_t bp_hi,
    uint16_t bp_lo
);

uint32_t aqi_pm25(uint16_t c_pm25) {
  if (c_pm25 < 13)  return _aqi(c_pm25, GOOD_LO, GOOD_HI,   0,  12);
  if (c_pm25 < 36)  return _aqi(c_pm25, MODT_LO, MODT_HI,  13,  35);
  if (c_pm25 < 56)  return _aqi(c_pm25, UFSG_LO, UFSG_HI,  36,  55);
  if (c_pm25 < 151) return _aqi(c_pm25, UNHY_LO, UNHY_HI,  56, 150);
  if (c_pm25 < 251) return _aqi(c_pm25, VUNH_LO, VUNH_HI, 151, 250);
  if (c_pm25 < 351) return _aqi(c_pm25, HAZ1_LO, HAZ1_HI, 251, 350);

  return _aqi(c_pm25, HAZ2_LO, HAZ2_HI, 351, 500);
}

uint32_t aqi_pm10(uint16_t c_pm10) {
  if (c_pm10 < 54)  return _aqi(c_pm10, GOOD_LO, GOOD_HI,   0,  54);
  if (c_pm10 < 155) return _aqi(c_pm10, MODT_LO, MODT_HI,  55, 154);
  if (c_pm10 < 255) return _aqi(c_pm10, UFSG_LO, UFSG_HI, 155, 254);
  if (c_pm10 < 355) return _aqi(c_pm10, UNHY_LO, UNHY_HI, 255, 354);
  if (c_pm10 < 425) return _aqi(c_pm10, VUNH_LO, VUNH_HI, 355, 424);
  if (c_pm10 < 505) return _aqi(c_pm10, HAZ1_LO, HAZ1_HI, 425, 504);

  return _aqi(c_pm10, HAZ2_LO, HAZ2_HI, 505, 604);
}

uint32_t _aqi(
    uint16_t c_p,
    uint16_t i_hi,
    uint16_t i_lo,
    uint16_t bp_hi,
    uint16_t bp_lo
) {
  // Do division in float space to retain accuracy.
  return (uint32_t) (((float) (i_hi - i_lo) / (bp_hi - bp_lo)) * (c_p - bp_lo) + i_lo);
}

