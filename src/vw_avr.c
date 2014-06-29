#include <avr/io.h>
#include <avr/interrupt.h>
#include "vw_avr.h"

static uint8_t vw_mode;

void vw_avr_setup(uint8_t mode) {
  // setup ports
  vw_mode = mode;

  if (mode & VW_AVR_MODE_RX) {
    // Set port as input
    VW_AVR_RX_DDR  &= ~(1 << VW_AVR_RX_PIN);
    VW_AVR_RX_PORT &= ~(1 << VW_AVR_RX_PIN);
  }

  if (mode & VW_AVR_MODE_TX) {
    // Set port as output
    VW_AVR_TX_DDR  |=  (1 << VW_AVR_TX_PIN);
    VW_AVR_TX_PORT &= ~(1 << VW_AVR_TX_PIN);
  }

  // setup timer
  TCCR0A = (1 << WGM01);     // CTC mode
  TCCR0B = VW_AVR_PRESCALER; // set prescaler
  OCR0A  = VW_AVR_COMPARE;   // set compare value
  TIMSK |= (1 << OCIE0A);    // interupt on compare match

  sei(); // enable interrupts
};
