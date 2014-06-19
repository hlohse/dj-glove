#include "Bluetooth.h"
#include "../Platform.h"
#include <SoftwareSerial.h>
using namespace Platform;

SoftwareSerial Bluetooth(bluetooth_pin_rx, bluetooth_pin_tx);

void BluetoothSetup()
{
    Bluetooth.begin(bluetooth_baud);
}

