#ifndef DISABLE_RX

#include <stdint.h>
#include "vw_pll.h"

static volatile uint8_t vw_pll_integrator  = 0;
static volatile uint8_t vw_pll_last_sample = 0;
static volatile uint8_t vw_pll_ramp        = 0;

void vw_pll(uint8_t sample, vw_pll_callback_t callback) {
  if (sample != vw_pll_last_sample) {
    if (vw_pll_ramp < VW_PLL_RAMP_TRANSITION) {
      vw_pll_ramp += VW_PLL_RAMP_RETARD;
    } else {
      vw_pll_ramp += VW_PLL_RAMP_ADVANCE;
    }
    vw_pll_last_sample = sample;
  } else {
    vw_pll_ramp += VW_PLL_RAMP_DEFAULT;
  }

  if (sample) {
    vw_pll_integrator++;
  }

  if (vw_pll_ramp >= VW_PLL_RAMP_LENGTH) {
    if (vw_pll_integrator >= VW_PLL_INTEGRATOR_LIMIT) {
      callback(1);
    } else {
      callback(0);
    }

    vw_pll_integrator = 0;
    vw_pll_ramp -= VW_PLL_RAMP_LENGTH;
  }
}

#ifdef TEST
void vw_pll_state(uint8_t** integrator, uint8_t** last_sample, uint8_t** ramp) {
  *integrator  = (uint8_t*) &vw_pll_integrator;
  *last_sample = (uint8_t*) &vw_pll_last_sample;
  *ramp        = (uint8_t*) &vw_pll_ramp;
}
#endif

#endif
