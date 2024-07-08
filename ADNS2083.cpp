#include "Arduino.h"
#include "OptiMouse.h"
#include "ADNS2083.h"

#define Delta_X				0x03
#define Delta_Y				0x04

ADNS2083::ADNS2083(uint8_t sclkPin, uint8_t sdioPin) : OptiMouse::OptiMouse(sclkPin, sdioPin){}

signed char ADNS2083::dx(void)
{
	return (signed char) readRegister(Delta_X);
}

signed char ADNS2083::dy(void)
{
	return (signed char) readRegister(Delta_Y);
}
signed char ADNS2083::mo(void)
{
	return (signed char) readRegister(0x02);
}
signed char ADNS2083::ms(void)
{
	return (signed char) readRegister(0x06);
}
void ADNS2083::config(uint8_t val){
  writeRegister(0x06, val);
}
// Private Methods /////////////////////////////////////////////////////////////