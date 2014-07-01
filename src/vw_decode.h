#ifndef VW_DECODE_H_
#define VW_DECODE_H_

#include <stdint.h>

// TODO limit encode length as well
#define VW_DECODE_MAX_LENGTH 80

#define VW_DECODE_STATE_NONE 0x0
#define VW_DECODE_STATE_DATA 0x1

typedef void (*vw_decode_callback_t)(uint8_t*);
typedef void (*vw_decode_bits_callback_t)(uint16_t*);
extern vw_decode_callback_t vw_decode_callback;
extern vw_decode_bits_callback_t vw_decode_bits_callback;

void vw_decode_bit(uint8_t bit);

#ifdef TEST
void vw_decode_get_state(uint8_t** bit_index, uint16_t** bits, uint8_t** buffer_index, uint8_t** buffer, uint8_t** state);
#endif

#endif
