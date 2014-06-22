#include "BtLatency.h"
#include <Bluetooth.h>
#include <SoftwareSerial.h>

int count = 0;
String message = "";

void setup()  
{
    BluetoothSetup();
}

void PingPong()
{
    for (; count < BtLatency::num_messages; ++count) {
        while (!Bluetooth.available());

        while (Bluetooth.available()) {
            message += (char) Bluetooth.read();
        }

        if (message == BtLatency::message) {
            Bluetooth.print(BtLatency::message);
            message = "";
        }
    }
}

void loop()
{
    if (Bluetooth.available()) {
        if (Bluetooth.read() == BtLatency::start_signal) {
            PingPong();
        }
    }
}

