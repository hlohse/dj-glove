#include "BtLatency.h"
#include <Bluetooth.h>
#include <SoftwareSerial.h>

void setup()  
{
    BluetoothSetup();
}

void loop()
{
    if (Bluetooth.available()) {
        if (Bluetooth.read() == BtLatency::start_signal) {
            for (unsigned int i = 0; i < BtLatency::num_messages; ++i) {
                Bluetooth.print(BtLatency::message);
            }
        }
    }
}

