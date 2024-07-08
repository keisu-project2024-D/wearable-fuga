#include "ADNS2083.h"
//#include "BleMouse.h"
#include "Mouse.h"
#define ENCODER_DO_NOT_USE_INTERRUPTS
#define SCLK 2                            // Serial clock pin on the Arduino
#define SDIO 3                            // Serial data (I/O) pin on the Arduino
#define LEFT 4
#define RIGHT 5

void mouseSetup();
void mouseScroll(double now);

void mouseControl();
void sendInformation(char x, char y, bool left, bool right);