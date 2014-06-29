#ifndef AVR_INTERRUPT_H_
#define AVR_INTERRUPT_H_

extern void TIMER0_COMPA_vect(void);

#define sei() 1
#define ISR(vector) void vector (void)

#endif
