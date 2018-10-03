#ifndef SERIAL_H
#define SERIAL_H

#include <inttypes.h>

class Serial {

public:
  virtual uint8_t write(uint8_t data[], uint8_t size) = 0;
};

#endif //SERIAL_H
