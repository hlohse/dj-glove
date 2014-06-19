/*
 *  This file provides information of the Arduino setup regarding pin usage,
 *  serial rates etc.
 *  Changes to the hardware platform must be reflected here accordingly.
 */

#ifndef DJ_GLOVE_ARDUINO_COMMON_PLATFORM_H_
#define DJ_GLOVE_ARDUINO_COMMON_PLATFORM_H_

namespace Platform {

const int bluetooth_pin_rx = 2;     // Arduino D pin connected to Bluetooth TXD
const int bluetooth_pin_tx = 3;     // Arduino D pin connected to Bluetooth RXD
const int bluetooth_baud   = 9600;  // Baud rate for Bluetooth serial TXD/RXD

}

#endif // DJ_GLOVE_ARDUINO_COMMON_PLATFORM_H_

