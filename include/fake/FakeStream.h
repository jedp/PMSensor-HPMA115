#ifndef INCLUDE_FAKESTREAM_H_
#define INCLUDE_FAKESTREAM_H_

#include <stddef.h>
#include <stdint.h>

class Stream {
 public:
   Stream();
   int available();
   int peek();
   int read();
   void readBytes(uint8_t *to, uint8_t len);

 private:
   uint8_t *buf = NULL;
   uint8_t buf_idx = 0;
   uint8_t buf_len = 0;
};

#endif  // INCLUDE_FAKESTREAM_H_

