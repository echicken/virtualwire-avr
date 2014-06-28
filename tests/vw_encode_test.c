#include "minunit.h"
#include "vw_encode.h"

static uint8_t test_data[] = { 0x3a, 0x95, 0x83, 0x64 };

int test_encoding_preamble() {
  uint8_t* buffer = vw_encode(test_data, 4);

  // Training preamble
  mu_assert_equal(buffer[0], 0x2a); // 101010
  mu_assert_equal(buffer[1], 0x2a); // 101010
  mu_assert_equal(buffer[2], 0x2a); // 101010
  mu_assert_equal(buffer[3], 0x2a); // 101010
  mu_assert_equal(buffer[4], 0x2a); // 101010
  mu_assert_equal(buffer[5], 0x2a); // 101010

  // Sync vector
  mu_assert_equal(buffer[6], 0x3a); // 111010
  mu_assert_equal(buffer[7], 0x2c); // 101100

  return 0;
}

int test_encoding_message_length() {
  uint8_t* buffer = vw_encode(test_data, 4);

  // Byte 8 and 9 are the packet length (7) encoded 4-to-6
  mu_assert_equal(buffer[8], 0xd);  // 0
  mu_assert_equal(buffer[9], 0x1c); // 7

  return 0;
}

int test_encoding_message_data() {
  uint8_t* buffer = vw_encode(test_data, 4);

  // Bytes 10 - 17 are the data encoded 4-to-6
  mu_assert_equal(buffer[10], 0x15); // 3
  mu_assert_equal(buffer[11], 0x26); // a

  mu_assert_equal(buffer[12], 0x25); // 9
  mu_assert_equal(buffer[13], 0x19); // 5

  mu_assert_equal(buffer[14], 0x23); // 8
  mu_assert_equal(buffer[15], 0x15); // 3

  mu_assert_equal(buffer[16], 0x1a); // 6
  mu_assert_equal(buffer[17], 0x16); // 4

  return 0;
}

int test_encoding_message_fcs() {
  uint8_t* buffer = vw_encode(test_data, 4);

  // Bytes 18 - 21 are the ones complement of the CCITT CRC-16, which is 0x5cb5.
  // Byte order is little endian.
  mu_assert_equal(buffer[18], 0x29); // b
  mu_assert_equal(buffer[19], 0x19); // 5
  mu_assert_equal(buffer[20], 0x19); // 5
  mu_assert_equal(buffer[21], 0x2a); // c

  return 0;
}

int test_encoding_null_terminated() {
  uint8_t* buffer = vw_encode(test_data, 4);

  mu_assert_equal(buffer[22], 0x0);

  return 0;
}

int all_tests() {
  mu_run_test(test_encoding_preamble);
  mu_run_test(test_encoding_message_length);
  mu_run_test(test_encoding_message_data);
  mu_run_test(test_encoding_message_fcs);
  mu_run_test(test_encoding_null_terminated);
  return 0;
}
