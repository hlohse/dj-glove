#include "BtLatency.h"
#include <Bluetooth.h>
#include <SoftwareSerial.h>

int count = 0;

void setup()  
{
    BluetoothSetup();
}

void loop()
{
    for (; count < BtLatency::num_messages; ++count) {
        Bluetooth.print(BtLatency::message);
    }
}

