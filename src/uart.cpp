#include "Uart.h"

#include <util/setbaud.h>
#include <avr/io.h>

/******************************/
// Public
/******************************/
Uart::Uart() {
  UBRR0H = UBRRH_VALUE;
  UBRR0L = UBRRL_VALUE;

  #if USE_2X
    UCSR0A |= (1 << U2X0);
  #else
    UCSR0A &= ~(1 << U2X0);
  #endif
}

uint8_t Uart::write(uint8_t data[], uint8_t size) {
  for(uint8_t i = 0; i < size; ++i) {
    while (!(UCSR0A & (1<<UDRE0)));
    UDR0 = data[i];
  }
  return 0;
}
