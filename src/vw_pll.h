#ifndef VW_PLL_H_
#define VW_PLL_H_
#ifndef DISABLE_RX

#include <stdint.h>

// PLL ramp configuration
#define VW_PLL_RAMP_LENGTH      160
#define VW_PLL_RAMP_TRANSITION  80 // VW_PLL_RAMP_LENGTH / 2
#define VW_PLL_RAMP_DEFAULT     32 // VW_PLL_RAMP_LENGTH / samples (5)
#define VW_PLL_RAMP_ADVANCE     57 // VW_PLL_RAMP_DEFAULT * 1.8
#define VW_PLL_RAMP_RETARD      17 // VW_PLL_RAMP_DEFAULT * 0.5
#define VW_PLL_INTEGRATOR_LIMIT 3

typedef void (*vw_pll_callback_t)(uint8_t);
extern void vw_pll(uint8_t sample, vw_pll_callback_t callback);

#ifdef TEST
extern void vw_pll_state(uint8_t** integrator, uint8_t** last_sample, uint8_t** ramp);
#endif

#endif
#endif
