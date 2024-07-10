#include "handkey.h"
#include "yubimouse.h"

void setup(){
  mouseSetup();
  //keyboardSetup();
}

void loop() {
  //keyboardControl();
  mouseControl();
  delay(5);
}