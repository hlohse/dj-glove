#include <Bluetooth.h>
#include <SoftwareSerial.h>

const int max_count = 1000;
int count = 0;

void setup()  
{
    BluetoothSetup();
}

void loop()
{
    for (; count < max_count; ++count) {
        Bluetooth.print("PING");
    }
}

