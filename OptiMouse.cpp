
#include "OptiMouse.h"

OptiMouse::OptiMouse(uint8_t SCLK, uint8_t SDIO)
{
  _sclkPin = SCLK;
  _sdioPin = SDIO;
  pinMode (_sclkPin, OUTPUT);
  pinMode (_sdioPin, INPUT);
}

void OptiMouse::begin(void)
{
	// Re-sync (see datasheet �5.4):
	// Toggle the SLCK line from high to low to high....
	digitalWrite(_sclkPin, HIGH);                     
	delayMicroseconds(5);
	digitalWrite(_sclkPin, LOW);
	delayMicroseconds(1);
	digitalWrite(_sclkPin, HIGH);
	
	// Wait at least tSIWTT (0.9 second?) for the
	// OptiMouse serial transaction timer to time out:
	delay(1000);
}
uint8_t OptiMouse::readRegister(uint8_t address)
{
	int i = 7;
	uint8_t r = 0;
	
	// Write the address of the register we want to read:
	pinMode (_sdioPin, OUTPUT);
	for (; i>=0; i--)
	{
		digitalWrite (_sclkPin, LOW);
		digitalWrite (_sdioPin, address & (1 << i));
		digitalWrite (_sclkPin, HIGH);
	}
	
	// Switch data line from OUTPUT to INPUT
	pinMode (_sdioPin, INPUT);
	
	// Wait a bit...
	delayMicroseconds(100);
	
	// Fetch the data!
	for (i=7; i>=0; i--)
	{                             
		digitalWrite (_sclkPin, LOW);
		digitalWrite (_sclkPin, HIGH);
		r |= (digitalRead (_sdioPin) << i);
	}
	delayMicroseconds(100);
	
	return r;
}

//レジスタ書き込み関数
void OptiMouse::writeRegister(uint8_t address, uint8_t data)
{
  int i = 7;
  
  // Set MSB high, to indicate write operation:
  address |= 0x80;
  
  // Write the address:
  pinMode (3, OUTPUT);
  for (; i>=0; i--)
  {
    digitalWrite (_sclkPin, LOW);
    digitalWrite (_sdioPin, address & (1 << i));
    digitalWrite (_sclkPin, HIGH);
  }
  
  // Write the data:
  for (i=7; i>=0; i--)
  {
    digitalWrite (_sclkPin, LOW);
    digitalWrite (_sdioPin, data & (1 << i));
    digitalWrite (_sclkPin, HIGH);
  }
}