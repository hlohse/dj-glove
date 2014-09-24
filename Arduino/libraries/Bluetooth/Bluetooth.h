#ifndef DJ_GLOVE_ARDUINO_LIBRARIES_BLUETOOTH_H_
#define DJ_GLOVE_ARDUINO_LIBRARIES_BLUETOOTH_H_

#include <Arduino.h>
#include <SoftwareSerial.h>

class Bluetooth : public SoftwareSerial {
public:
  static const int default_pin_rx = 2;
  static const int default_pin_tx = 3;
  static const int default_baud   = 9600;

  Bluetooth()
  : SoftwareSerial(default_pin_rx, default_pin_tx, default_baud)
  {
	begin(default_baud);
  }

  Bluetooth(const byte rx, const byte tx)
  : SoftwareSerial(rx, tx)
  {
	begin(default_baud);
  }

  Bluetooth(const byte rx, const byte tx, const int baud)
  : SoftwareSerial(rx, tx)
  {
    begin(baud);
  }
};

#endif // DJ_GLOVE_ARDUINO_LIBRARIES_BLUETOOTH_H_

