#include <avr/io.h>
#include <avr/pgmspace.h>

#include "uart.h"

#define BAUD 19200

void uart_init(void) {
  // set baud rate
  UBRRH = 0x00;
  UBRRL = ((F_CPU / 16) / BAUD) - 1;

  // enable receiver and transmitter
  UCSRB |= (1<<RXEN) | (1<<TXEN);

  // frame format: async, 8 data bits, no parity, 1 stop bit
  UCSRC |= (1<<URSEL) | (1<<UCSZ1) | (1<<UCSZ0);
}

void uart_newline(void) {
  uart_sendchar('\x1B');
  uart_sendchar('E');
}

void uart_sendchar(char data) {
  // wait for empty transmit buffer
  while (!(UCSRA & (1<<UDRE))) {}

  // put data on buffer and send 
  UDR = data;
}

unsigned char uart_readchar() {
  // wait for data
  if (UCSRA & (1<<RXC)) {
    return UDR;
  } else {
    return 0;
  }
}

void uart_sendstr(char *data) 
{
  while (*data) {
    uart_sendchar(*data);
    data++;
  }
}

void uart_sendstr_p(const char *progmem_s)
{
  // print string from program memory
  char c;
  while ((c = pgm_read_byte(progmem_s++))) {
    uart_sendchar(c);
  }
}
