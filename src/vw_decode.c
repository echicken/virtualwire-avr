#ifndef DISABLE_RX

#include "vw_decode.h"
#include "vw_pll.h"
#include "vw_4to6.h"

static volatile uint8_t  vw_decode_bit_index = 0;
static volatile uint16_t vw_decode_bits;
static volatile uint8_t  vw_decode_buffer_index = 0;
static volatile uint8_t  vw_decode_buffer_length = 0;
static volatile uint8_t  vw_decode_buffer[VW_DECODE_MAX_LENGTH];
static volatile uint8_t  vw_decode_state = VW_DECODE_STATE_NONE;

vw_decode_callback_t vw_decode_callback;

void vw_decode_bit(uint8_t bit) {
  vw_decode_bit_index++;
  vw_decode_bits >>= 1;

  if (bit) {
    vw_decode_bits |= 0x800;
  }

  if (vw_decode_state == VW_DECODE_STATE_DATA && vw_decode_bit_index >= 12) {
    vw_decode_bit_index = 0;
    vw_4to6_decode(vw_decode_bits, (uint8_t*) &vw_decode_buffer, (uint8_t*) &vw_decode_buffer_index);

    if (vw_decode_buffer_index == 1) {
      vw_decode_buffer_length = vw_decode_buffer[0];
    } else if (vw_decode_buffer_index >= vw_decode_buffer_length) {
      vw_pll_set_mode(VW_PLL_MODE_PRE);
      vw_decode_state = VW_DECODE_STATE_NONE;
      if (vw_decode_callback) {
        vw_decode_callback((uint8_t*) &vw_decode_buffer);
      }
    }
  } else if (vw_decode_state == VW_DECODE_STATE_NONE && vw_decode_bits == 0xb3a) {
    vw_pll_set_mode(VW_PLL_MODE_DATA);
    vw_decode_bit_index    = 0;
    vw_decode_buffer_index = 0;
    vw_decode_state = VW_DECODE_STATE_DATA;
  }
}

#ifdef TEST
void vw_decode_get_state(uint8_t** bit_index, uint16_t** bits, uint8_t** buffer_index, uint8_t** buffer, uint8_t** state) {
  *bit_index    = (uint8_t*)  &vw_decode_bit_index;
  *bits         = (uint16_t*) &vw_decode_bits;
  *buffer_index = (uint8_t*)  &vw_decode_buffer_index;
  *buffer       = (uint8_t*)  &vw_decode_buffer;
  *state        = (uint8_t*)  &vw_decode_state;
}
#endif

#endif
