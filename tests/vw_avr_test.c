#include <avr/reset.h>
#include <avr/interrupt.h>
#include "minunit.h"
#include "vw_avr.h"

int test_setup_ports() {
  // transmitter mode
  mock_avr_reset();
  vw_avr_setup(VW_AVR_MODE_TX);
  mu_assert_bit(VW_AVR_RX_DDR,  VW_AVR_RX_PIN, 0);
  mu_assert_bit(VW_AVR_RX_PORT, VW_AVR_RX_PIN, 0);
  mu_assert_bit(VW_AVR_TX_DDR,  VW_AVR_TX_PIN, 1); // set as output
  mu_assert_bit(VW_AVR_TX_PORT, VW_AVR_TX_PIN, 0); // held low

  // receiver mode
  mock_avr_reset();
  vw_avr_setup(VW_AVR_MODE_RX);
  mu_assert_bit(VW_AVR_RX_DDR,  VW_AVR_RX_PIN, 0); // set as input
  mu_assert_bit(VW_AVR_RX_PORT, VW_AVR_RX_PIN, 0); // tri-state
  mu_assert_bit(VW_AVR_TX_DDR,  VW_AVR_TX_PIN, 0);
  mu_assert_bit(VW_AVR_TX_PORT, VW_AVR_TX_PIN, 0);

  // transceiver mode
  mock_avr_reset();
  vw_avr_setup(VW_AVR_MODE_TX | VW_AVR_MODE_RX);
  mu_assert_bit(VW_AVR_RX_DDR,  VW_AVR_RX_PIN, 0); // set as input
  mu_assert_bit(VW_AVR_RX_PORT, VW_AVR_RX_PIN, 0); // tri-state
  mu_assert_bit(VW_AVR_TX_DDR,  VW_AVR_TX_PIN, 1); // set as output
  mu_assert_bit(VW_AVR_TX_PORT, VW_AVR_TX_PIN, 0); // held low

  return 0;
}

int test_setup_timer() {
  mock_avr_reset();
  vw_avr_setup(VW_AVR_MODE_TX | VW_AVR_MODE_RX);

  // prescaler set
  mu_assert_equal(TCCR0B, VW_AVR_PRESCALER);

  // CTC mode enabled
  mu_assert_equal(TCCR0A & (1 << WGM01), 0x2);

  // compare value set
  mu_assert_equal(OCR0A, VW_AVR_COMPARE)

  // interrupt enabled
  mu_assert_equal(TIMSK & (1 << OCIE0A), 0x10);

  return 0;
}

int test_send() {
  uint8_t* buffer = malloc(4);
  buffer[0] = 0xd;
  buffer[1] = 0x19;
  buffer[2] = 0x15;
  buffer[3] = 0;

  mock_avr_reset();
  vw_avr_setup(VW_AVR_MODE_TX);

  mu_assert_equal(vw_avr_tx_in_progress(), 0);
  vw_avr_send(buffer);
  mu_assert_equal(vw_avr_tx_in_progress(), 1);

  // each bit is transmitted on the timer interrupt

  // 0xd - 101100
  TIMER0_COMPA_vect();
  mu_assert_bit(VW_AVR_TX_PORT, VW_AVR_TX_PIN, 1);
  TIMER0_COMPA_vect();
  mu_assert_bit(VW_AVR_TX_PORT, VW_AVR_TX_PIN, 0);
  TIMER0_COMPA_vect();
  mu_assert_bit(VW_AVR_TX_PORT, VW_AVR_TX_PIN, 1);
  TIMER0_COMPA_vect();
  mu_assert_bit(VW_AVR_TX_PORT, VW_AVR_TX_PIN, 1);
  TIMER0_COMPA_vect();
  mu_assert_bit(VW_AVR_TX_PORT, VW_AVR_TX_PIN, 0);
  TIMER0_COMPA_vect();
  mu_assert_bit(VW_AVR_TX_PORT, VW_AVR_TX_PIN, 0);

  // 0x19 - 100110
  TIMER0_COMPA_vect();
  mu_assert_bit(VW_AVR_TX_PORT, VW_AVR_TX_PIN, 1);
  TIMER0_COMPA_vect();
  mu_assert_bit(VW_AVR_TX_PORT, VW_AVR_TX_PIN, 0);
  TIMER0_COMPA_vect();
  mu_assert_bit(VW_AVR_TX_PORT, VW_AVR_TX_PIN, 0);
  TIMER0_COMPA_vect();
  mu_assert_bit(VW_AVR_TX_PORT, VW_AVR_TX_PIN, 1);
  TIMER0_COMPA_vect();
  mu_assert_bit(VW_AVR_TX_PORT, VW_AVR_TX_PIN, 1);
  TIMER0_COMPA_vect();
  mu_assert_bit(VW_AVR_TX_PORT, VW_AVR_TX_PIN, 0);

  // 0x15 - 101010
  TIMER0_COMPA_vect();
  mu_assert_bit(VW_AVR_TX_PORT, VW_AVR_TX_PIN, 1);
  TIMER0_COMPA_vect();
  mu_assert_bit(VW_AVR_TX_PORT, VW_AVR_TX_PIN, 0);
  TIMER0_COMPA_vect();
  mu_assert_bit(VW_AVR_TX_PORT, VW_AVR_TX_PIN, 1);
  TIMER0_COMPA_vect();
  mu_assert_bit(VW_AVR_TX_PORT, VW_AVR_TX_PIN, 0);
  TIMER0_COMPA_vect();
  mu_assert_bit(VW_AVR_TX_PORT, VW_AVR_TX_PIN, 1);
  TIMER0_COMPA_vect();
  mu_assert_bit(VW_AVR_TX_PORT, VW_AVR_TX_PIN, 0);

  // 0x0 - null terminated
  mu_assert_equal(vw_avr_tx_in_progress(), 1);

  TIMER0_COMPA_vect();
  mu_assert_bit(VW_AVR_TX_PORT, VW_AVR_TX_PIN, 0);
  mu_assert_equal(vw_avr_tx_in_progress(), 0);

  TIMER0_COMPA_vect();
  mu_assert_bit(VW_AVR_TX_PORT, VW_AVR_TX_PIN, 0);

  TIMER0_COMPA_vect();
  mu_assert_bit(VW_AVR_TX_PORT, VW_AVR_TX_PIN, 0);

  return 0;
}

int all_tests() {
  mu_run_test(test_setup_ports);
  mu_run_test(test_setup_timer);
  mu_run_test(test_send);
  return 0;
}
