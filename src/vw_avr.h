#ifndef VW_AVR_H_
#define VW_AVR_H_

#include <avr/io.h>

// Port configuration
#define VW_AVR_TX_PORT PORTB
#define VW_AVR_TX_DDR  DDRB
#define VW_AVR_TX_PIN  PINB1

#if __AVR_ATmega8__
#define VW_AVR_RX_PORT PINB
#define VW_AVR_RX_DDR  DDRB
#define VW_AVR_RX_PIN  PINB0
#else
#define VW_AVR_RX_PORT PORTB
#define VW_AVR_RX_DDR  DDRB
#define VW_AVR_RX_PIN  PINB2
#endif

// Timer configuration
// If we want a speed of 9600bps we need to fire the interupt that many times:
//
// Prescaler 1024 at 1MHz will fire the timer 1000000/1024 = 976 times per
// second, so that is two slow.
//
// Prescaler 256 at 1MHz will fire the timer 1000000/256 = 3906 times per
// second, so that is two slow.
//
// Prescaler 64 at 1Mhz will fire the timer 1000000/64 = 15625 times per
// second. That is two high, but we can also set the compare register,
// 15625/9600 = 1.62. We can't set it accurately enough with that though.
//
// Prescaler 8 at 1Mhz will fire the timer 1000000/8 = 125000 times per
// second. That is two high, but we can also set the compare register,
// 125000/9600 = 13.02. That'll give us 9615bps.
//
// Prescaler 1 at 1Mhz will fire the timer 1000000/1 = 1000000 times per
// second. That is two high, but we can also set the compare register,
// 1000000/9600 = 104.17. That'll also give us 9615bps. As the previous
// settings fire the timer slower we'll choose that.
#if __AVR_ATmega8__
// RX loop 4032 times per second
#define VW_AVR_PRESCALER (1 << CS01) // Prescaler 8
#define VW_AVR_COMPARE   10
#else
// 500 baud rate
#define VW_AVR_PRESCALER (1 << CS01) // Prescaler 8
#define VW_AVR_COMPARE   250
#endif

// Mode flags
#define VW_AVR_MODE_TX   0x01
#define VW_AVR_MODE_RX   0x02

extern void    vw_avr_setup(uint8_t mode);
extern void    vw_avr_disable();
extern uint8_t vw_avr_tx_in_progress();
extern void    vw_avr_send(uint8_t* buffer);

#endif
