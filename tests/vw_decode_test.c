#include "minunit.h"
#include "vw_decode.h"

static uint8_t*  bit_index;
static uint16_t* bits;
static uint8_t*  buffer_index;
static uint8_t*  buffer;
static uint8_t*  state;

void decode_reset() {
  vw_decode_get_state(&bit_index, &bits, &buffer_index, &buffer, &state);
  *bit_index    = 0;
  *buffer_index = 0;
  *state        = VW_DECODE_STATE_NONE;
}

int test_decode_bits() {
  decode_reset();
  mu_assert_equal(*bit_index, 0);

  vw_decode_bit(1);
  mu_assert_equal(*bit_index, 1);
  mu_assert_equal(*bits, 0x800);

  vw_decode_bit(1);
  mu_assert_equal(*bit_index, 2);
  mu_assert_equal(*bits, 0xc00);

  vw_decode_bit(0);
  mu_assert_equal(*bit_index, 3);
  mu_assert_equal(*bits, 0x600);

  vw_decode_bit(1);
  mu_assert_equal(*bit_index, 4);
  mu_assert_equal(*bits, 0xb00);

  return 0;
}

int test_decode_start_vector() {
  decode_reset();

  *bit_index    = 6;
  *buffer_index = 3;
  *bits         = 0x674;
  mu_assert_equal(*state, VW_DECODE_STATE_NONE);

  vw_decode_bit(1);
  mu_assert_equal(*bit_index, 0);
  mu_assert_equal(*buffer_index, 0);
  mu_assert_equal(*bits, 0xb3a);
  mu_assert_equal(*state, VW_DECODE_STATE_DATA);

  return 0;
}

int test_decoded_byte_added_to_buffer() {
  decode_reset();

  *bit_index = 11;
  *bits      = 0xb58;
  *state     = VW_DECODE_STATE_DATA;

  mu_assert_equal(*buffer_index, 0);
  mu_assert_equal(*buffer, 0);

  vw_decode_bit(0);
  mu_assert_equal(*bit_index, 0);
  mu_assert_equal(*bits, 0x5ac);
  mu_assert_equal(*buffer_index, 1);
  mu_assert_equal(*buffer, 0xd4);

  return 0;
}

int all_tests() {
  mu_run_test(test_decode_bits);
  mu_run_test(test_decode_start_vector);
  mu_run_test(test_decoded_byte_added_to_buffer);

  return 0;
}
