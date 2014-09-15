#ifndef DJ_GLOVE_ARDUINO_DJ_GLOVE_PINS_H_
#define DJ_GLOVE_ARDUINO_DJ_GLOVE_PINS_H_

struct Pins {
  static const byte bt_rx   =  2; // Connected to TXD
  static const byte bt_tx   =  3; // Connected to RXD
  static const byte poti_0  = A0;
  static const byte poti_1  = A0; // TODO
  static const byte poti_2  = A0; // TODO
  static const byte push_0  =  3;
  static const byte push_1  =  3; // TODO
  static const byte push_2  =  3; // TODO
  static const byte push_3  =  3; // TODO
  static const byte push_4  =  3; // TODO
  static const byte touch_0 =  3; // TODO
  static const byte touch_1 =  3; // TODO
  static const byte touch_2 =  3; // TODO
  static const byte touch_3 =  3; // TODO
  static const byte flip    =  3; // TODO
  static const byte flex    = A7;
  static const byte led_clk = A3;
  static const byte led_sda = A2;
};

#endif // DJ_GLOVE_ARDUINO_DJ_GLOVE_PINS_H_

