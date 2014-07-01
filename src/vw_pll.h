#ifndef VW_PLL_H_
#define VW_PLL_H_
#ifndef DISABLE_RX

#include <stdint.h>

// PLL modes
#define VW_PLL_MODE_PRE           0x0
#define VW_PLL_MODE_LOCKED        0x1
#define VW_PLL_MODE_DATA          0x2

#define VW_PLL_CONFIDENCE_THRESHOLD 250
#define VW_PLL_CONFIDENCE_MAX       255
#define VW_PLL_CONFIDENCE_MIN       0
#define VW_PLL_CONFIDENCE_DIVISOR   6
#define VW_PLL_LOCK_MAX             100
#define VW_PLL_RAMP_LENGTH          180
#define VW_PLL_RAMP_ADVANCE         1.8
#define VW_PLL_RAMP_RETARD          0.2

typedef void (*vw_pll_callback_t)(uint8_t);
extern void vw_pll(uint8_t sample, vw_pll_callback_t callback);
extern void vw_pll_set_mode(uint8_t mode);

#ifdef TEST
extern void vw_pll_state(uint8_t** integrator, uint8_t** last_sample, uint8_t** clock, uint8_t** current_clock, uint8_t** confidence, uint8_t** lock_counter, uint8_t** mode);
#endif

#endif
#endif
