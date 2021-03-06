#include <assert.h>
#include "fake/FakeStream.h"

Stream::Stream() {
}

int Stream::available() {
  return tx_end - tx_idx;
}

int Stream::peek() {
  return tx_buf[tx_idx];
}

int Stream::read() {
  return tx_buf[tx_idx++];
}

void Stream::readBytes(uint8_t to[], uint8_t len) {
  for (uint8_t i = 0; i < len; ++i) {
    to[i] = tx_buf[tx_idx + i];
  }
  tx_idx += len;
}

void Stream::write(uint8_t from[], uint8_t len) {
  for (uint8_t i = 0; i < len; ++i) {
    rx_buf[rx_end++] = from[i];
  }

  // Respond to commands.
  if (rx_buf[rx_idx++] == 0x68) {
    if (rx_buf[rx_idx++] == 0x01) {
      uint8_t cmd = rx_buf[rx_idx++];

      // Read particle measurement results
      if (cmd == 0x04) {
        // Checksum. See datasheet, Table 6, for expected values.
        if (rx_buf[rx_idx++] != 0x93) {
          add(0x96); add(0x96);
          return;
        } else {
          // Return example reading from data sheet.
          add(0x40); add(0x0D); add(0x04);              // Headers
          add(0x00); add(0x30);                         // PM 1
          add(0x00); add(0x31);                         // PM 2.5
          add(0x00); add(0x32);                         // PM 4
          add(0x00); add(0x33);                         // PM 10
          add(0x00); add(0x00); add(0x00); add(0x00);   // Reserved
          add(0xE9);                                    // Checksum
          return;
        }
      }

      // Stop measurement
      if (cmd == 0x02) {
        // Checksum. See datasheet, Table 6, for expected values.
        if (rx_buf[rx_idx++] == 0x95) {
          add(0xA5); add(0xA5);
          return;
        } else {
          add(0x96); add(0x96);
          return;
        }
      }

      // Start measurement
      if (cmd == 0x01) {
        // Checksum. See datasheet, Table 6, for expected values.
        if (rx_buf[rx_idx++] == 0x96) {
          add(0xA5); add(0xA5);
          return;
        } else {
          add(0x96); add(0x96);
          return;
        }
      }

      // Stop auto-send
      if (cmd == 0x20) {
        // Checksum. See datasheet, Table 6, for expected values.
        if (rx_buf[rx_idx++] == 0x77) {
          add(0xA5); add(0xA5);
          return;
        } else {
          add(0x96); add(0x96);
          return;
        }
      }

      // Start auto-send
      if (cmd == 0x40) {
        // Checksum. See datasheet, Table 6, for expected values.
        if (rx_buf[rx_idx++] == 0x57) {
          add(0xA5); add(0xA5);
          return;
        } else {
          add(0x96); add(0x96);
          return;
        }
      }
    }
  }

  assert(0);  // We didn't understand the input. Abort.
}

void Stream::add(uint8_t byte) {
  tx_buf[tx_end++] = byte;
}

uint8_t Stream::take() {
  return rx_buf[rx_idx++];
}

