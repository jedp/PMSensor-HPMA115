#ifndef INCLUDE_HPM_HPMA115_COMPACT_H_
#define INCLUDE_HPM_HPMA115_COMPACT_H_

#ifdef UNIT_TEST
#include "fake/FakeStream.h"
#else
#include <Stream.h>
#endif

#include <stddef.h>
#include "HPMA115_Common.h"

/*
 * Byte sequence for the autosend data format from the compact series,
 * HPMA115C0-003 and HPMA115C0-004. (Table 7 in the Honeywell data sheet.)
 *
 * Index of bytes, omitting the two header bytes.
 */
typedef enum {
  LEN_H = 0,
  LEN_L,
  DATA0_H,
  DATA0_L,
  DATA1_H,
  DATA1_L,
  DATA2_H,
  DATA2_L,
  DATA3_H,
  DATA3_L,
  CS_H = 28,
  CS_L,
  AUTO_SEND_DATA_BYTES,
} compact_autosend_state_t;

/*
 * A reading from the compact sensor.
 */
typedef struct {
  uint16_t pm1;
  uint16_t pm25;
  uint16_t pm4;
  uint16_t pm10;
  uint16_t aqi;
  uint16_t checksum;
} compact_auto_result_t;

typedef enum {
  NEW_DATA = 0,
  NO_DATA,
  NOT_HEAD0_RETRY,
  NOT_HEAD1_RETRY,
  INSUFFICIENT_DATA,
  BAD_CHECKSUM,
  DEVICE_UNAVAILABLE = 255
} compact_auto_status_t;

class HPMA115_Compact {
 public:
  HPMA115_Compact();

  /*
   * Configure the instance to use this stream to communicate with the STM.
   */
  bool begin(Stream *stream);

  /*
   * Check for data received. Return status code.
   *
   * If this returns NEW_DATA, then you can access the new data by calling
   * the getPM25() or  getAQI(), etc.
   */
  compact_auto_status_t checkAutoReceive();

  /*
   * Stop the HPM from automatically sending results.
   *
   * Blocks until HPM responds with success code. Returns true on success,
   * false otherwise.
   */
  bool stopAutoSend();

  /*
   * Tell the HPM to start sending results automatically.
   *
   * Blocks until HPM responds with success code. Returns true on success,
   * false otherwise.
   */
  bool startAutoSend();

  /* Get the most recent PM 1.0 reading. */
  uint16_t getPM1();

  /* Get the most recent PM 2.5 reading. */
  uint16_t getPM25();

  /* Get the most recent PM 4.0 reading. */
  uint16_t getPM4();

  /* Get the most recent PM 10.0 reading. */
  uint16_t getPM10();

  /*
   * Get the most recent AQI reading.
   *
   * Based on the higher of the PM 2.5 and PM 10.0 readings.
   */
  uint16_t getAQI();

 private:
  /* The stream connected to the STM UART. */
  Stream *hpma = NULL;

  /* Latest readings. */
  compact_auto_result_t result_data = { 0xFF };
};

#endif  // INCLUDE_HPM_HPMA115_COMPACT_H_

