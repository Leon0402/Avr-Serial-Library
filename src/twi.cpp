#include "Twi.h"

#include <util/twi.h>
#include <avr/io.h>

/******************************/
// Public
/******************************/
Twi::Twi(uint32_t clockFrequenzy, bool useInternalPullups) {
  // I2C needs pullup resistors, which can be external or internal
  if(useInternalPullups) {
    PORTC = (1 << PINC5) | (1 << PINC4);
  }

  // Clear Prescaler Bits TWPS0 & TWPS1 in TWSR to set Prescale to 1
  TWSR = (0 << TWPS0) | (0 << TWPS1);
  // Adjusts Bit Rate Generator to get the right twi Frequency
  TWBR = ((F_CPU/clockFrequenzy)-16)/2;

  TWCR = (1 << TWEN) | (1 << TWEA);
}

uint8_t Twi::write(uint8_t data[], uint8_t size) {
  //send start condition bit
  TWCR = (1 << TWEN) | (1 << TWEA) | (1 << TWINT) | ( 1 << TWSTA);
  //wait until start has been send
  while(!(TWCR & (1<<TWINT)));
  //check if start has been send sucessfully
  if((TW_STATUS != TW_START) && (TW_STATUS != TW_REP_START)) {
    return TW_STATUS;
  }

  //write address of slave and write modus into Data Register
  TWDR = TW_WRITE | (this->address << 1);
  //send content of data register
  TWCR = (1 << TWEN) | (1 << TWEA) | (1 << TWINT);
  //wait until content of data has been send
  while (!(TWCR & (1<<TWINT)));
  //check if address exits (slave answered)
  if (TW_STATUS != TW_MT_SLA_ACK) {
    return TW_STATUS;
  }

  //write data into Data Register
  for(uint8_t i = 0; i < size; ++i) {
    TWDR = data[i];
    //send ack
    TWCR = (1 << TWEN) | (1 << TWEA) | ( 1 << TWINT);
    //wait until ack has been send
    while(!(TWCR & (1<<TWINT)));
    //check if ack has been received by slave
    if (TW_STATUS != TW_MT_DATA_ACK) {
      return TW_STATUS;
    }
  }
  return 0;
}

void Twi::stop() {
  //send stop condition bit
  TWCR =  (1<<TWEN) | (1<<TWEA) | (1<<TWINT) | (1<<TWSTO);
  //wait until top has been send -> Stop does not set TWINT
  while(TWCR & (1 << TWSTO));
}

void Twi::setAddress(uint8_t address) {
  this->address = address;
}
