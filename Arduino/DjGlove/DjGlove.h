#ifndef DJ_GLOVE_ARDUINO_DJ_GLOVE_DJ_GLOVE_H_
#define DJ_GLOVE_ARDUINO_DJ_GLOVE_DJ_GLOVE_H_

#include "Pins.h"
#include "Push.h"
#include "Poti.h"
#include "Flex.h"
#include "Led.h"

// Note: Instantiate only once
struct DjGlove {
  Poti poti_0;
  Poti poti_1;
  Poti poti_2;
  Push push_0;
  Push push_1;
  Push push_2;
  Push push_3;
  Push push_4;
  Flex flex;
  Led  led;
  byte channel;
  byte program;
  
  DjGlove()
  : poti_0(Pins::poti_0),
    poti_1(Pins::poti_1),
    poti_2(Pins::poti_2),
    push_0(Pins::push_0),
    push_1(Pins::push_1),
    push_2(Pins::push_2),
    push_3(Pins::push_3),
    push_4(Pins::push_4),
    flex(Pins::flex),
    led(Pins::led_clk, Pins::led_sda),
    channel(0),
    program(0)
  {
  }
};

#endif // DJ_GLOVE_ARDUINO_DJ_GLOVE_DJ_GLOVE_H_

