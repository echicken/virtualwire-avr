#ifndef UART_H
#define UART_H

extern void uart_init(void);
extern void uart_sendchar(char data);
extern void uart_sendstr(char *data);
extern void uart_sendstr_p(const char *progmem_s);
extern void uart_newline();
extern unsigned char uart_readchar();

#endif
