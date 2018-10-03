#ifndef TWI_H
#define TWI_H

#include "Serial.h"

class Twi : public Serial {

public:
  Twi(uint32_t clockFrequenzy, bool useInternalPullups);

  uint8_t write(uint8_t data[], uint8_t size) override;
  void stop();

  void setAddress(uint8_t address);

private:
  uint8_t address;
};
#endif //TWI_H
