#ifndef DISABLE_RX

#include <stdint.h>
#include "vw_pll.h"

static volatile uint8_t vw_pll_integrator      = 0;
static volatile uint8_t vw_pll_last_sample     = 0;
static volatile uint8_t vw_pll_clock           = 0;
static volatile uint8_t vw_pll_confidence      = 0;
static volatile uint8_t vw_pll_lock_counter    = 0;
static volatile uint8_t vw_pll_ramp            = 0;
static volatile uint8_t vw_pll_ramp_length     = 0;
static volatile uint8_t vw_pll_ramp_transition = 0;
static volatile uint8_t vw_pll_ramp_default    = 0;
static volatile uint8_t vw_pll_ramp_advance    = 0;
static volatile uint8_t vw_pll_ramp_retard     = 0;
static volatile uint8_t vw_pll_mode            = VW_PLL_MODE_PRE;

extern void print_bit(uint8_t bit);
extern void pll_mode(uint8_t bit);

void vw_pll_set_mode(uint8_t mode) {
  vw_pll_mode       = mode;
  vw_pll_confidence = 0;
  print_bit(3);
  pll_mode(mode);
}

void vw_pll(uint8_t sample, vw_pll_callback_t callback) {
  if (vw_pll_mode == VW_PLL_MODE_PRE) {
    vw_pll_ramp++;
    //print_bit(sample);

    if (sample != vw_pll_last_sample) {
      vw_pll_last_sample = sample;

      uint8_t clock_offset = (vw_pll_ramp - vw_pll_clock) * 2 / 4;

      vw_pll_clock += clock_offset;

      if (clock_offset < 1) {
        if (vw_pll_confidence < VW_PLL_CONFIDENCE_THRESHOLD) {
          vw_pll_confidence += (VW_PLL_CONFIDENCE_MAX - vw_pll_confidence) / VW_PLL_CONFIDENCE_DIVISOR;
        } else {
          vw_pll_mode         = VW_PLL_MODE_LOCKED;
          vw_pll_lock_counter = 0;
          vw_pll_integrator   = 0;

          // ensure the length is divisable by vw_pll_clock
          vw_pll_ramp_length     = (VW_PLL_RAMP_LENGTH / vw_pll_clock) * vw_pll_clock;
          vw_pll_ramp_transition = vw_pll_ramp_length / 2;
          vw_pll_ramp_default    = vw_pll_ramp_length / vw_pll_clock;
          vw_pll_ramp_advance    = vw_pll_ramp_default * VW_PLL_RAMP_ADVANCE;
          vw_pll_ramp_retard     = vw_pll_ramp_default * VW_PLL_RAMP_RETARD;

          print_bit(2);
        }
      } else {
        if (vw_pll_confidence > VW_PLL_CONFIDENCE_MIN) {
          vw_pll_confidence -= vw_pll_confidence / 2;
        }
      }

      vw_pll_ramp = 0;
    }
  } else {
    if (sample) {
      vw_pll_integrator++;
    }

    //print_bit(sample);

    if (sample != vw_pll_last_sample) {
      if (vw_pll_ramp < vw_pll_ramp_transition) {
        vw_pll_ramp += vw_pll_ramp_retard;
      } else {
        vw_pll_ramp += vw_pll_ramp_advance;
      }
      vw_pll_last_sample = sample;
    } else {
      vw_pll_ramp += vw_pll_ramp_default;
    }

    if (vw_pll_ramp >= vw_pll_ramp_length) {
      print_bit(4);
      if (vw_pll_integrator >= (vw_pll_clock / 2)) {
        callback(1);
      } else {
        callback(0);
      }

      vw_pll_ramp          -= vw_pll_ramp_length;
      vw_pll_integrator    = 0;

      if (vw_pll_mode == VW_PLL_MODE_LOCKED) {
        if(vw_pll_lock_counter++ >= VW_PLL_LOCK_MAX) {
          vw_pll_mode = VW_PLL_MODE_PRE;
          vw_pll_confidence = 0;
          vw_pll_ramp = 0;
          print_bit(3);
        }
      }
    }
  }
}

#ifdef TEST
void vw_pll_state(uint8_t** integrator, uint8_t** last_sample, uint8_t** clock, uint8_t** current_clock, uint8_t** confidence, uint8_t** lock_counter, uint8_t** mode) {
  *integrator    = (uint8_t*) &vw_pll_integrator;
  *last_sample   = (uint8_t*) &vw_pll_last_sample;
  *clock         = (uint8_t*) &vw_pll_clock;
  *current_clock = (uint8_t*) &vw_pll_ramp;
  *confidence    = (uint8_t*) &vw_pll_confidence;
  *lock_counter  = (uint8_t*) &vw_pll_lock_counter;
  *mode          = (uint8_t*) &vw_pll_mode;
}
#endif

#endif
