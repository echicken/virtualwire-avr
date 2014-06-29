#ifndef MINUNIT_H_
#define MINUNIT_H_

#include <stdio.h>
#include <stdlib.h>

int result;

#define mu_assert(test, message) { \
  if (!(test)) { \
    printf("Line %d, assertion failed: %s\n", __LINE__, message); \
    return 1; \
  } \
}

#define mu_assert_equal(actual, expected) { \
  if ((actual) != (expected)) { \
    printf("Line %d, expected 0x%x to equal 0x%x\n", __LINE__, actual, expected); \
    return 1; \
  } \
}

#define mu_assert_bit(container, bit, set_unset) { \
  uint8_t value = (container & (1 << bit)); \
  if (set_unset && !value) { \
    printf("Line %d, expected %s to be set\n", __LINE__, #bit); \
    return 1; \
  } else if (!set_unset && value) { \
    printf("Line %d, expected %s to not be set\n", __LINE__, #bit); \
    return 1; \
  } \
}

#define mu_run_test(test) { \
  result = test(); \
  if (result) { \
    return result; \
  } \
}

extern int all_tests();

int main() {
  return all_tests();
}

#endif
