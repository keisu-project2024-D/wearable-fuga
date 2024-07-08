#ifndef ADNS2083_h
#define ADNS2083_h

#include "OptiMouse.h"

class ADNS2083 : public OptiMouse
{
  private:
  public:
	ADNS2083(uint8_t, uint8_t);
	signed char dx(void);
	signed char dy(void);
	signed char mo(void);
	signed char ms(void);
  void config(uint8_t val);
};

#endif
