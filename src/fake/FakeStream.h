#ifndef SRC_FAKE_FAKESTREAM_H_
#define SRC_FAKE_FAKESTREAM_H_

#include <stdint.h>

const uint8_t BUF_LEN = 32;

class Stream {
 public:
  Stream();
  int available();
  int peek();
  int read();
  void readBytes(uint8_t *to, uint8_t len);
  void write(uint8_t *from, uint8_t len);

  /* Add a byte onto the fake TX stream. */
  void add(uint8_t byte);
  /* Take a byte from the fake RX stream. */
  uint8_t take();

 private:
  uint8_t tx_buf[BUF_LEN] = { 0 };
  uint8_t tx_idx = 0;
  uint8_t tx_end = 0;

  uint8_t rx_buf[BUF_LEN] = { 0 };
  uint8_t rx_idx = 0;
  uint8_t rx_end = 0;
};

#endif  // SRC_FAKE_FAKESTREAM_H_

