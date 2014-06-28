#ifndef VW_CRC16_H_
#define VW_CRC16_H_

#ifdef __AVR__
  // Use AVR optimised functions if available
  #include <util/crc16.h>
#else
  #include <stdint.h>

  extern uint16_t _crc_ccitt_update(uint16_t crc, uint8_t data);
#endif

#endif
