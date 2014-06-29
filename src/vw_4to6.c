#include "vw_4to6.h"

static uint8_t symbols[] =
{
// 0 8   1 9   2 A   3 B   4 C   5 D   6 E   7 F
   0xd,  0xe,  0x13, 0x15, 0x16, 0x19, 0x1a, 0x1c,
   0x23, 0x25, 0x26, 0x29, 0x2a, 0x2c, 0x32, 0x34
};

// Encodes the given 8 bits of data as two 6 bit symbols. High nibble then low
// nibble. Index is a counter to the position in the buffer, which is
// incremented by 2 on each call.
void vw_4to6_encode(uint8_t data, uint8_t* buffer, uint8_t* index) {
  buffer[(*index)++] = symbols[data >> 4];
  buffer[(*index)++] = symbols[data & 0xf];
}
