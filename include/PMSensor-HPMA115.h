#ifndef SRC_HPMA_H_
#define SRC_HPMA_H_

#include <stdint.h>

#define HPMA115_BAUD 9600

/* General commands for the HPMA115 series. */
enum command {
  START_PM    = 0x01,  // Start particle measurement; also starts fan.
  STOP_PM     = 0x02,  // Stop particle measurement; also shuts down fan.
  READ_PM     = 0x04,  // Read particle measuing results.
  SET_ADJ     = 0x08,  // Set customer adjustment coefficient. Range 30-200.
  GET_ADJ     = 0x10,  // Get customer adjustment coefficient. Default 100.
  STOP_AUTO   = 0x20,  // Stop automatically sending results.
  START_AUTO  = 0x40,  // Start automatically sending results.
};

/* General control and response bytes for the HPMA115 series. */
enum header {
  RECV        = 0x40,  // Response: Positive ACK with data.
  SEND        = 0x68,  // Send command.
  NACK        = 0x96,  // Negative ACK. Repeated.
  ACK         = 0xA5,  // Positive ACK. Repeated.
};

/*
 * Byte sequence for the autosend data format from the compact series,
 * HPMA115C0-003 and HPMA115C0-004. (Table 7 in the Honeywell data sheet.)
 */
typedef enum {
  HEAD_0,
  HEAD_1,
  LEN_H,
  LEN_L,
  DATA0_H,
  DATA0_L,
  DATA1_H,
  DATA1_L,
  DATA2_H,
  DATA2_L,
  DATA3_H,
  DATA3_L,
  DATA_RESERVED,
  CS_H,
  CS_L,
  UNKNOWN,
} compact_autosend_state_t;

#endif  // SRC_HPMA_H_

