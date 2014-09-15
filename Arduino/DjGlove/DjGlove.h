#ifndef DJ_GLOVE_ARDUINO_DJ_GLOVE_DJ_GLOVE_H_
#define DJ_GLOVE_ARDUINO_DJ_GLOVE_DJ_GLOVE_H_

#include "Pins.h"

// Note: Instantiate only once
struct DjGlove {
  Poti poti_0;
  Push push_0;
  Led  led;
  byte channel;
  byte program;
  
  DjGlove()
  : poti_0(Pins::poti_0),
    push_0(Pins::push_0),
    led(Pins::led_clk, Pins::led_sda),
    channel(0),
    program(0)
  {
  }
};

#endif // DJ_GLOVE_ARDUINO_DJ_GLOVE_DJ_GLOVE_H_

