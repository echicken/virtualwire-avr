#include <avr/pgmspace.h>
#include <stdlib.h>
#include <vw_avr.h>
#include <vw_pll.h>
#include <vw_decode.h>
#include "uart.h"

unsigned char msg_buf[20];
uint8_t packet = 0;

void print_packet(uint8_t* rx_buffer) {
  uint8_t length = rx_buffer[0] - 3;

  if (length > 150) {
    uart_sendchar('E');
    return;
  }

  uart_newline();
  uart_sendstr_p(PSTR("Packet: "));
  utoa(packet++, (char *)msg_buf, 10);
  uart_sendstr((char *)msg_buf);
  uart_newline();
  uart_sendstr_p(PSTR("Data Length: "));
  utoa(length, (char *)msg_buf, 10);
  uart_sendstr((char *)msg_buf);
  uart_newline();

  uart_sendstr_p(PSTR("Dec\tHex\tChr"));
  uart_newline();

  uint8_t i;
  for (i = 1; i <= length; i++) {
    utoa(rx_buffer[i], (char *)msg_buf, 10);
    uart_sendstr((char *)msg_buf);
    uart_sendchar('\t');

    utoa(rx_buffer[i], (char *)msg_buf, 16);
    uart_sendstr((char *)msg_buf);
    uart_sendchar('\t');

    uart_sendchar(rx_buffer[i]);
    uart_newline();
  }
  uart_newline();
}

uint8_t counter = 0;

void print_bit(uint8_t bit) {
    //return;
    if (bit) {
        uart_sendchar('1');
    } else {
        uart_sendchar('0');
    }

    if (counter++ > 150) {
        counter = 0;
        uart_newline();
    }
}

void pll_mode(uint8_t mode) {
    switch(mode) {
        case VW_PLL_MODE_PRE:
          uart_sendchar('P');
          break;
        case VW_PLL_MODE_LOCKED:
          uart_sendchar('L');
          break;
        case VW_PLL_MODE_DATA:
          uart_sendchar('D');
          break;
    }
}

int main(void)
{
  uart_init();
  int i = 0;

  for (i = 0; i < 20; i++) {
    uart_newline();
  }
  uart_sendstr_p(PSTR("===== VirtualWire Receiver ====="));
  uart_newline();

  vw_avr_setup(VW_AVR_MODE_RX);
  vw_decode_callback = &print_packet;

  while(1) {}
  return 0;
}
