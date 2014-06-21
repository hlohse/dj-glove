#include "Bluetooth.h"
#include "../Platform.h"
#include <SoftwareSerial.h>

SoftwareSerial Bluetooth(Platform::bluetooth_pin_rx,
                         Platform::bluetooth_pin_tx);

void BluetoothSetup()
{
    Bluetooth.begin(Platform::bluetooth_baud);
}

