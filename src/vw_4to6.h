#ifndef VW_4TO6_H_
#define VW_4TO6_H_

#include <stdint.h>

extern void vw_4to6_encode(uint8_t data, uint8_t* buffer, uint8_t* index);

#ifndef DISABLE_RX
extern void vw_4to6_decode(uint16_t data, uint8_t* buffer, uint8_t* index);
#endif

#endif
