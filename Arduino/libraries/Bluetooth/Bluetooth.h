#ifndef DJ_GLOVE_ARDUINO_LIBRARIES_BLUETOOTH_H_
#define DJ_GLOVE_ARDUINO_LIBRARIES_BLUETOOTH_H_

#include <Arduino.h>
#include <SoftwareSerial.h>

class Bluetooth : public SoftwareSerial {
public:
  static const int pin_rx = 2;
  static const int pin_tx = 3;
  static const int baud = 9600;

  Bluetooth()
  : Bluetooth(pin_rx, pin_tx)
  {
  }

  Bluetooth(const byte rx, const byte tx)
  : SoftwareSerial(rx, tx)
  {
    begin(baud);
  }
};

#endif // DJ_GLOVE_ARDUINO_LIBRARIES_BLUETOOTH_H_

