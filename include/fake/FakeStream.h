#ifndef INCLUDE_FAKE_FAKESTREAM_H_
#define INCLUDE_FAKE_FAKESTREAM_H_

#include <stdint.h>

const uint8_t BUF_LEN = 32;

class Stream {
 public:
  Stream();
  int available();
  int peek();
  int read();
  void readBytes(uint8_t *to, uint8_t len);

  /*Add a byte onto the fake stream. */
  void add(uint8_t byte);

 private:
  uint8_t buf[BUF_LEN] = { 0 };
  uint8_t idx = 0;
  uint8_t end = 0;
};

#endif  // INCLUDE_FAKE_FAKESTREAM_H_

