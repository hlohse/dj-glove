/*
 *  This file provides a SoftwareSerial for Bluetooth communication by using
 *  platform's pin and baud configuration.
 *
 *  Example usage (loopback):
 *  
 *      BluetoothSetup();
 *      
 *      if (Bluetooth.available()) {
 *           Bluetooth.print(Bluetooth.read());
 *      }
 */

#ifndef DJ_GLOVE_ARDUINO_LIBRARIES_BLUETOOTH_H_
#define DJ_GLOVE_ARDUINO_LIBRARIES_BLUETOOTH_H_

class SoftwareSerial;

// Use as regular SoftwareSerial for communication via Bluetooth
extern SoftwareSerial Bluetooth;

// Call once, e.g. in setup function
void BluetoothSetup();

#endif // DJ_GLOVE_ARDUINO_LIBRARIES_BLUETOOTH_H_

