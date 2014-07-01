#include "minunit.h"
#include "vw_pll.h"

static uint8_t* integrator;
static uint8_t* last_sample;
static uint8_t* clock;
static uint8_t* current_clock;
static uint8_t* confidence;
static uint8_t* lock_counter;
static uint8_t* mode;
static uint8_t  callback_ran;
static uint8_t  callback_bit;
static uint8_t  i;
static uint8_t  j;

void pll_callback(uint8_t bit) {
  callback_bit = bit;
  callback_ran = 1;
}

void pll_reset() {
  vw_pll_state(&integrator, &last_sample, &clock, &current_clock, &confidence, &lock_counter, &mode);
  *integrator    = 0;
  *last_sample   = 0;
  *clock         = 0;
  *current_clock = 0;
  *confidence    = 0;
  *mode          = VW_PLL_MODE_PRE;
  callback_ran   = 0;
  callback_bit   = 0;
}

int test_basic_clock_recovery() {
  pll_reset();

  for(i = 0; i < 4; i++)
    vw_pll(0, &pll_callback);

  for(i = 0; i < 4; i++)
    vw_pll(1, &pll_callback);

  for(i = 0; i < 4; i++)
    vw_pll(0, &pll_callback);

  for(i = 0; i < 4; i++)
    vw_pll(1, &pll_callback);

  mu_assert_equal(*clock, 3);
  mu_assert_equal(*confidence, 63);

  for(i = 0; i < 4; i++)
    vw_pll(0, &pll_callback);

  mu_assert_equal(*clock, 3);
  mu_assert_equal(*confidence, 111); // confidence increases

  pll_reset();

  for(i = 0; i < 8; i++)
    vw_pll(0, &pll_callback);

  for(i = 0; i < 8; i++)
    vw_pll(1, &pll_callback);

  for(i = 0; i < 8; i++)
    vw_pll(0, &pll_callback);

  for(i = 0; i < 8; i++)
    vw_pll(1, &pll_callback);

  for(i = 0; i < 8; i++)
    vw_pll(0, &pll_callback);

  mu_assert_equal(*clock, 7);
  mu_assert_equal(*confidence, 63);

  for(i = 0; i < 8; i++)
    vw_pll(1, &pll_callback);

  mu_assert_equal(*clock, 7);
  mu_assert_equal(*confidence, 111); // confidence increases

  return 0;
}

int test_data_integration_on_clock() {
  pll_reset();
  vw_pll_set_mode(VW_PLL_MODE_DATA);
  *clock = 4;

  for(i = 0; i < 3; i++)
    vw_pll(0, &pll_callback);

  mu_assert_equal(callback_ran, 0);
  mu_assert_equal(callback_bit, 0);

  vw_pll(0, &pll_callback);

  mu_assert_equal(callback_ran, 1);
  mu_assert_equal(callback_bit, 0);

  callback_ran = 0;
  callback_bit = 0;

  for(i = 0; i < 3; i++)
    vw_pll(1, &pll_callback);

  mu_assert_equal(callback_ran, 0);
  mu_assert_equal(callback_bit, 0);

  vw_pll(1, &pll_callback);

  mu_assert_equal(callback_ran, 1);
  mu_assert_equal(callback_bit, 1);

  return 0;
}

int test_data_integration_on_clock_without_edge() {
  pll_reset();
  vw_pll_set_mode(VW_PLL_MODE_DATA);
  *clock = 4;

  for(i = 0; i < 3; i++)
    vw_pll(1, &pll_callback);

  mu_assert_equal(callback_ran, 0);
  mu_assert_equal(callback_bit, 0);

  vw_pll(1, &pll_callback);

  mu_assert_equal(callback_ran, 1);
  mu_assert_equal(callback_bit, 1);

  callback_ran = 0;
  callback_bit = 0;

  for(i = 0; i < 3; i++)
    vw_pll(1, &pll_callback);

  mu_assert_equal(callback_ran, 0);
  mu_assert_equal(callback_bit, 0);

  vw_pll(1, &pll_callback);

  mu_assert_equal(callback_ran, 1);
  mu_assert_equal(callback_bit, 1);

  return 0;
}

int test_data_integration() {
  pll_reset();
  vw_pll_set_mode(VW_PLL_MODE_DATA);
  *clock = 4;

  vw_pll(0, &pll_callback);
  vw_pll(0, &pll_callback);
  vw_pll(0, &pll_callback);
  vw_pll(0, &pll_callback);

  mu_assert_equal(callback_bit, 0);

  vw_pll(1, &pll_callback);
  vw_pll(0, &pll_callback);
  vw_pll(1, &pll_callback);
  vw_pll(1, &pll_callback);

  mu_assert_equal(callback_bit, 1);

  vw_pll(0, &pll_callback);
  vw_pll(1, &pll_callback);
  vw_pll(0, &pll_callback);
  vw_pll(0, &pll_callback);

  mu_assert_equal(callback_bit, 0);

  vw_pll(0, &pll_callback);
  vw_pll(1, &pll_callback);
  vw_pll(1, &pll_callback);
  vw_pll(0, &pll_callback);

  mu_assert_equal(callback_bit, 1);

  return 0;
}

int test_locks_when_confident() {
  pll_reset();

  for(j = 0; j < 10; j++) {
    for(i = 0; i < 4; i++)
      vw_pll(0, &pll_callback);

    for(i = 0; i < 4; i++)
      vw_pll(1, &pll_callback);
  }

  mu_assert_equal(*mode, VW_PLL_MODE_LOCKED);

  return 0;
}

int test_doesnt_lock_when_not_confident() {
  pll_reset();

  for(j = 0; j < 6; j++) {
    for(i = 0; i < 4; i++)
      vw_pll(0, &pll_callback);

    for(i = 0; i < 4; i++)
      vw_pll(1, &pll_callback);
  }

  mu_assert_equal(*mode, VW_PLL_MODE_PRE);

  vw_pll(0, &pll_callback);
  vw_pll(0, &pll_callback);
  vw_pll(1, &pll_callback);
  vw_pll(1, &pll_callback);
  vw_pll(1, &pll_callback);
  vw_pll(0, &pll_callback);
  vw_pll(0, &pll_callback);
  vw_pll(1, &pll_callback);
  vw_pll(1, &pll_callback);
  vw_pll(0, &pll_callback);
  vw_pll(0, &pll_callback);
  vw_pll(0, &pll_callback);

  mu_assert_equal(*mode, VW_PLL_MODE_PRE);

  for(j = 0; j < 4; j++) {
    for(i = 0; i < 4; i++)
      vw_pll(0, &pll_callback);

    for(i = 0; i < 4; i++)
      vw_pll(1, &pll_callback);
  }

  mu_assert_equal(*mode, VW_PLL_MODE_PRE);

  return 0;
}

int test_unlocks_without_mode_switch() {
  pll_reset();
  vw_pll_set_mode(VW_PLL_MODE_LOCKED);
  *clock = 4;

  for(j = 0; j < 50; j++)
    for(i = 0; i < 4; i++)
      vw_pll(0, &pll_callback);

  mu_assert_equal(*mode, VW_PLL_MODE_PRE);

  return 0;
}

int all_tests() {
  mu_run_test(test_basic_clock_recovery);
  mu_run_test(test_data_integration_on_clock);
  mu_run_test(test_data_integration_on_clock_without_edge);
  mu_run_test(test_data_integration);
  mu_run_test(test_locks_when_confident);
  mu_run_test(test_doesnt_lock_when_not_confident);
  mu_run_test(test_unlocks_without_mode_switch);
  return 0;
}
