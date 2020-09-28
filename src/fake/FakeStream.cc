#include <assert.h>
#include "fake/FakeStream.h"

Stream::Stream() {
}

int Stream::available() {
  return end - idx;
}

int Stream::peek() {
  return buf[idx];
}

int Stream::read() {
  return buf[idx++];
}

void Stream::readBytes(uint8_t to[], uint8_t num_bytes) {
  for (uint8_t i = 0; i < num_bytes; ++i) {
    to[i] = buf[idx + i];
  }
  idx += num_bytes;
}

void Stream::add(uint8_t byte) {
  buf[end++] = byte;
}

