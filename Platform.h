/*      Platform
 *  This file provides information of the Arduino setup regarding pin usage,
 *  serial rates etc., as well as addresses etc.
 *  Changes to the hardware platform must be reflected here accordingly.
 */

#ifndef DJ_GLOVE_PLATFORM_H_
#define DJ_GLOVE_PLATFORM_H_

namespace Platform {

const int   bluetooth_pin_rx  = 2;     // D pin connected to Bluetooth module's TXD
const int   bluetooth_pin_tx  = 3;     // D pin connected to Bluetooth module's RXD
const int   bluetooth_baud    = 9600;  // Baud rate for Bluetooth module
const int   bluetooth_channel = 1;     // Channel the Bluetooth module uses
const char* bluetooth_mac     = "98:D3:31:B3:0A:25";  // Bluetooth module's MAC

}

#endif // DJ_GLOVE_PLATFORM_H_

