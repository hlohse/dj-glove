#ifndef DJ_GLOVE_ARDUINO_DJ_GLOVE_DJ_GLOVE_H_
#define DJ_GLOVE_ARDUINO_DJ_GLOVE_DJ_GLOVE_H_

#include "Pins.h"
#include "Button.h"
#include "Poti.h"
#include "Flex.h"
#include "Led.h"

// Note: Instantiate only once
struct DjGlove {
  Button push_0;
  Button push_1;
  Button push_2;
  Button push_3;
  Button push_4;
  Button touch_0;
  Button touch_1;
  Button touch_2;
  Button touch_3;
  Button flip;
  Poti   poti_0;
  Poti   poti_1;
  Poti   poti_2;
  Flex   flex;
  Led    led;
  byte   channel;
  byte   program;
  
  DjGlove()
  : push_0(Pins::push_0),
    push_1(Pins::push_1),
    push_2(Pins::push_2),
    push_3(Pins::push_3),
    push_4(Pins::push_4),
    touch_0(Pins::touch_0),
    touch_1(Pins::touch_1),
    touch_2(Pins::touch_2),
    touch_3(Pins::touch_3),
    flip(Pins::flip),
    poti_0(Pins::poti_0),
    poti_1(Pins::poti_1),
    poti_2(Pins::poti_2),
    flex(Pins::flex),
    led(Pins::led_clk, Pins::led_sda),
    channel(0),
    program(0)
  {
  }
};

#endif // DJ_GLOVE_ARDUINO_DJ_GLOVE_DJ_GLOVE_H_

