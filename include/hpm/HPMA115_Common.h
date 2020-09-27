#ifndef INCLUDE_HPM_HPMA115_COMMON_H_
#define INCLUDE_HPM_HPMA115_COMMON_H_

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

enum {
  AUTO_HEAD_0 = 0x42,  // First byte of auto-send data header.
  AUTO_HEAD_1 = 0x4D,  // Second byte of auto-send data header.
};

#endif  // INCLUDE_HPM_HPMA115_COMMON_H_

