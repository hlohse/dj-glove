#include <Bluetooth.h>
#include <SoftwareSerial.h>

void setup()  
{
    BluetoothSetup();
}

void loop()
{
    bool found_pong = false;
    char pong[4];

    Bluetooth.println("PING");

    while (!found_pong) {
        while (!Bluetooth.available());

        if (Bluetooth.available() >= 4) {
            Bluetooth.readBytes(pong, 4);

            if (pong[0] == 'P'
            &&  pong[1] == 'O'
            &&  pong[2] == 'N'
            &&  pong[3] == 'G')
            {
                found_pong = true;
            }
        }
    }
}

