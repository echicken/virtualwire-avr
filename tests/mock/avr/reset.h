#ifndef AVR_RESET_H_
#define AVR_RESET_H_

#include <avr/io.h>

void mock_avr_reset() {
  DDRB   = 0;
  PORTB  = 0;
  TCCR0A = 0;
  TCCR0B = 0;
  OCR0A  = 0;
  TIMSK  = 0;
}

#endif
