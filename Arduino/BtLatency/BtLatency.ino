#include <Bluetooth.h>
#include <SoftwareSerial.h>

// NOTE: USE THIS VALUES IN PC PROJECT, TOO!
const unsigned int num_messages = 65535;
const char*        message      = "PING";
const char         start_signal = '!';

void setup()  
{
    BluetoothSetup();
}

void loop()
{
    if (Bluetooth.available()) {
        if (Bluetooth.read() == start_signal) {
            for (unsigned int i = 0; i < num_messages; ++i) {
                Bluetooth.print(message);
            }
        }
    }
}

