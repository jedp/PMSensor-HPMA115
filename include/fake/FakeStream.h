#ifndef INCLUDE_FAKE_FAKESTREAM_H_
#define INCLUDE_FAKE_FAKESTREAM_H_

#include <stdint.h>

class Stream {
 public:
  Stream();
  int available();
  int peek();
  int read();
  void readBytes(uint8_t *to, uint8_t len);
};

#endif  // INCLUDE_FAKE_FAKESTREAM_H_

