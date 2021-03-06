#ifndef DJ_GLOVE_ARDUINO_DJ_GLOVE_CONFIG_H_
#define DJ_GLOVE_ARDUINO_DJ_GLOVE_CONFIG_H_

#include <Arduino.h>

struct Pin {
  static const byte poti_0      = A0;
  static const byte poti_1      = A1;
  static const byte poti_2      = A6;
  static const byte push_0      =  3;
  static const byte push_1      =  4;
  static const byte push_2      =  5;
  static const byte push_3      =  7;
  static const byte push_4      =  8;
  static const byte touch_0     =  9;
  static const byte touch_1     = 10;
  static const byte touch_2     = 11;
  static const byte touch_3     = 12;
  static const byte flip        =  6;
  static const byte flex        = A7;
  static const byte led_clk     = A3;
  static const byte led_sda     = A2;
  static const byte acc_int     =  2;
};

struct Address {
  static const byte ultra_sound = 112;
  static const byte gyro        = 0x69;
  static const byte acc         = 0x1C;
};

#endif // DJ_GLOVE_ARDUINO_DJ_GLOVE_CONFIG_H_

