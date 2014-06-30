#include <util/delay.h>
#include <avr/pgmspace.h>
#include <vw_avr.h>
#include <vw_encode.h>

int main(void)
{
  vw_avr_setup(VW_AVR_MODE_TX);

  while(1) {
    vw_avr_send(vw_encode("Hello world!", 13));

    _delay_ms(250);
    _delay_ms(250);
    _delay_ms(250);
    _delay_ms(250);
    _delay_ms(250);
    _delay_ms(250);
    _delay_ms(250);
    _delay_ms(250);
    _delay_ms(250);
    _delay_ms(250);
    _delay_ms(250);
    _delay_ms(250);
    _delay_ms(250);
    _delay_ms(250);
    _delay_ms(250);
    _delay_ms(250);
    _delay_ms(250);
    _delay_ms(250);
    _delay_ms(250);
    _delay_ms(250);
  }

  return 0;
}
