#ifndef __AVR__
// Replacement for optimised AVR CRC functions on other platforms

#include "vw_crc16.h"

uint16_t _crc_ccitt_update(uint16_t crc, uint8_t data)
{
    data ^= crc & 0xff;
    data ^= data << 4;
    return ((((uint16_t)data << 8) | (crc >> 8)) ^ (uint8_t)(data >> 4) ^ ((uint16_t)data << 3));
}

#endif
