#include "minunit.h"
#include "vw_pll.h"

static uint8_t* integrator;
static uint8_t* last_sample;
static uint8_t* ramp;
static volatile uint8_t callback_ran;
static volatile uint8_t callback_bit;

void pll_callback(uint8_t bit) {
  callback_bit = bit;
  callback_ran = 1;
}

void pll_reset() {
  vw_pll_state(&integrator, &last_sample, &ramp);
  *integrator  = 0;
  *last_sample = 0;
  *ramp        = 0;
  callback_ran = 0;
  callback_bit = 0;
}

int test_sanity_check() {
  // check the PLL configuration is as expected for tests
  mu_assert(VW_PLL_RAMP_DEFAULT * 3 <= VW_PLL_RAMP_LENGTH, "Ramp length too short");
  mu_assert(VW_PLL_RAMP_TRANSITION > VW_PLL_RAMP_ADVANCE, "Transition too short");
  mu_assert(VW_PLL_RAMP_DEFAULT > 10, "Default ramp increment too short");
  return 0;
}

int test_ramp_increases_each_sample() {
  pll_reset();
  *ramp = 0;
  *last_sample = 1;

  vw_pll(1, &pll_callback);
  mu_assert_equal(*ramp, VW_PLL_RAMP_DEFAULT);

  vw_pll(1, &pll_callback);
  mu_assert_equal(*ramp, VW_PLL_RAMP_DEFAULT * 2);

  vw_pll(1, &pll_callback);
  mu_assert_equal(*ramp, VW_PLL_RAMP_DEFAULT * 3);

  return 0;
}

int test_ramp_retards_before_transition() {
  uint8_t start = VW_PLL_RAMP_DEFAULT;

  pll_reset();
  *ramp = start;
  *last_sample = 1;

  vw_pll(0, &pll_callback);
  mu_assert_equal(*ramp, start + VW_PLL_RAMP_RETARD);

  return 0;
}

int test_ramp_advances_after_transition() {
  uint8_t start = VW_PLL_RAMP_TRANSITION + 1;

  pll_reset();
  *ramp = start;
  *last_sample = 1;

  vw_pll(0, &pll_callback);
  mu_assert_equal(*ramp, start + VW_PLL_RAMP_ADVANCE);

  return 0;
}

int test_ramp_overflow_triggers_callback() {
  pll_reset();
  *ramp = VW_PLL_RAMP_LENGTH - VW_PLL_RAMP_ADVANCE - 10;
  *last_sample = 1;

  mu_assert_equal(callback_ran, 0);

  vw_pll(1, &pll_callback);
  mu_assert_equal(callback_ran, 0);

  vw_pll(1, &pll_callback);
  mu_assert_equal(callback_ran, 0);

  vw_pll(1, &pll_callback);
  mu_assert_equal(callback_ran, 1);

  return 0;
}

int test_integrator() {
  pll_reset();
  *ramp = 0;
  *last_sample = 0;

  vw_pll(0, &pll_callback);
  mu_assert_equal(callback_ran, 0);

  while(1) {
    vw_pll(1, &pll_callback);
    if (callback_ran) {
      break;
    }
  }

  mu_assert_equal(callback_bit, 1);

  *ramp = 0;
  *last_sample = 1;
  callback_ran = 0;

  vw_pll(1, &pll_callback);
  mu_assert_equal(callback_ran, 0);

  while(1) {
    vw_pll(0, &pll_callback);
    if (callback_ran) {
      break;
    }
  }

  mu_assert_equal(callback_bit, 0);

  return 0;
}

int all_tests() {
  mu_run_test(test_sanity_check);
  mu_run_test(test_ramp_increases_each_sample);
  mu_run_test(test_ramp_retards_before_transition);
  mu_run_test(test_ramp_advances_after_transition);
  mu_run_test(test_ramp_overflow_triggers_callback);
  mu_run_test(test_integrator);

  return 0;
}
