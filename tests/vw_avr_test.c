#include <avr/reset.h>
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

int all_tests() {
  mu_run_test(test_setup_ports);
  mu_run_test(test_setup_timer);
  return 0;
}
