#ifndef UART_H
#define UART_H

#include "Serial.h"

#ifndef BAUD
  #define BAUD 9600
#endif

class Uart : public Serial {

public:
  Uart();

  uint8_t write(uint8_t data[], uint8_t size) override;
};
#endif //UART_H
