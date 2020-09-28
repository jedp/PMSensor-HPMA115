#ifndef SRC_HPM_HPMA115_COMMON_H_
#define SRC_HPM_HPMA115_COMMON_H_

#include <stdint.h>

#define HPMA115_BAUD 9600

enum {
  AUTO_HEAD_0 = 0x42,  // First byte of auto-send data header.
  AUTO_HEAD_1 = 0x4D,  // Second byte of auto-send data header.
  CMD_HEAD    = 0x68,  // Header byte of a command.
};

#endif  // SRC_HPM_HPMA115_COMMON_H_

