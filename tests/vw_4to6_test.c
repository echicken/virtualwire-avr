#include "minunit.h"
#include "vw_4to6.h"

int test_encoding_symbols() {
  uint8_t buffer[4] = { 0 };
  uint8_t index = 0;

  vw_4to6_encode(0x3a, (uint8_t*) &buffer, &index);

  mu_assert_equal(buffer[0], 0x15); // 3
  mu_assert_equal(buffer[1], 0x26); // a
  mu_assert_equal(buffer[2], 0);
  mu_assert_equal(buffer[3], 0);

  mu_assert_equal(index, 2);

  return 0;
}

int test_encode_offset() {
  uint8_t buffer[4] = { 0 };
  uint8_t index = 1;

  vw_4to6_encode(0x3a, (uint8_t*) &buffer, &index);

  mu_assert_equal(buffer[0], 0);
  mu_assert_equal(buffer[1], 0x15); // 3
  mu_assert_equal(buffer[2], 0x26); // a
  mu_assert_equal(buffer[3], 0);

  mu_assert_equal(index, 3);

  return 0;
}

int test_decoding_symbols() {
  uint8_t buffer[2] = { 0 };
  uint8_t index = 0;

  vw_4to6_decode(0x5ac, (uint8_t*) &buffer, &index);

  mu_assert_equal(buffer[0], 0xd4);
  mu_assert_equal(buffer[1], 0);

  return 0;
}

int test_decode_offset() {
  uint8_t buffer[2] = { 0 };
  uint8_t index = 0;

  vw_4to6_decode(0x5ac, (uint8_t*) &buffer, &index);
  vw_4to6_decode(0x5ac, (uint8_t*) &buffer, &index);

  mu_assert_equal(buffer[0], 0xd4);
  mu_assert_equal(buffer[1], 0xd4);

  mu_assert_equal(index, 2);

  return 0;
}

int test_encoding_and_decoding() {
  uint8_t buffer[2] = { 0 };
  uint8_t index = 0;

  vw_4to6_encode(0xe6, (uint8_t*) &buffer, &index);

  mu_assert_equal(buffer[0], 0x32); // e
  mu_assert_equal(buffer[1], 0x1a); // 6

  uint16_t encoded = (buffer[1] << 6) | buffer[0];

  buffer[0] = buffer[1] = 0;
  index = 0;

  vw_4to6_decode(encoded, (uint8_t*) &buffer, &index);
  mu_assert_equal(buffer[0], 0xe6);
  mu_assert_equal(buffer[1], 0);

  return 0;
}

int all_tests() {
  mu_run_test(test_encoding_symbols);
  mu_run_test(test_encode_offset);
  mu_run_test(test_decoding_symbols);
  mu_run_test(test_decode_offset);
  mu_run_test(test_encoding_and_decoding);
  return 0;
}
