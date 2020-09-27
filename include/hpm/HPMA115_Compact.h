#ifndef SRC_HPMA_COMPACT_H_
#define SRC_HPMA_COMPACT_H_

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
     * Configure the instance to use this stream to communicate with the STM
     * UART, and this data object to update with new readings.
     */
    bool begin(Stream *stream, compact_auto_result_t *data);

    /*
     * Check for data received. Update values in the result_data object
     * if data was successfully read. Return status code.
     */
    compact_auto_status_t checkAutoReceive();

  private:
    /* Auto-send is enabled. (Assume true; this is the device default.) */
    bool autosend = true;

    /* The stream connected to the STM UART. */
    Stream *hpma = NULL;

    /* The object we should update with new auto-send readings. */
    compact_auto_result_t *result_data = NULL;
};

#endif

