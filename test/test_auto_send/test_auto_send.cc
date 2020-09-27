#include <unity.h>

#include "hpm/HPMA115_Compact.h"
#include "fake/FakeStream.h"

void test_init(void) {
  HPMA115_Compact hpm = HPMA115_Compact();
  Stream fakeStream;

  hpm.begin(&fakeStream);
}

int main(int argc, char** argv) {
  UNITY_BEGIN();

  test_init();

  UNITY_END();
}

