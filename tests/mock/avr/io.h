#ifndef AVR_IO_H_
#define AVR_IO_H_

#include <stdint.h>

// ports
uint8_t DDRB;
uint8_t PORTB;

#define PINB1 0x1
#define PINB2 0x2

// timers
uint8_t TCCR0A;
uint8_t TCCR0B;
uint8_t OCR0A;
uint8_t TIMSK;

#define WGM01  0x1
#define OCIE0A 0x4

#endif
