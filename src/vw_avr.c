#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "vw_avr.h"

static volatile uint8_t  vw_mode;

static volatile uint8_t* tx_buffer;
static volatile uint8_t  tx_in_progress = 0;
static volatile uint8_t  tx_byte_index;
static volatile uint8_t  tx_bit_index;

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

void vw_avr_send(uint8_t* buffer) {
  if (vw_mode & VW_AVR_MODE_TX) {
    tx_buffer      = buffer;
    tx_in_progress = 1;
    tx_byte_index  = 0;
    tx_bit_index   = 0;
  }
};

uint8_t vw_avr_tx_in_progress() {
  return tx_in_progress;
}

void vw_avr_end_transmit() {
  // set low to stop transmitting
  VW_AVR_TX_PORT &= ~(1 << VW_AVR_TX_PIN);
  tx_in_progress = 0;
  free((void*) tx_buffer);
}

void vw_avr_transmit() {
  // finish transmitting if we are at the end of the buffer
  if (tx_buffer[tx_byte_index] == 0) {
    vw_avr_end_transmit();
  } else {
    if (tx_buffer[tx_byte_index] & (1 << tx_bit_index++)) {
      // bit is 1, set high
      VW_AVR_TX_PORT |= (1 << VW_AVR_TX_PIN);
    } else {
      // bit is 0, set low
      VW_AVR_TX_PORT &= ~(1 << VW_AVR_TX_PIN);
    }

    // increase the byte counter if we are at the end of the 6 bit byte
    if (tx_bit_index >= 6) {
      tx_byte_index++;
      tx_bit_index = 0;
    }
  }
}

ISR(TIMER0_COMPA_vect) {
  if (tx_in_progress) {
    vw_avr_transmit();
  }
}
