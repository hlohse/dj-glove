#include <Bluetooth.h>
#include <SoftwareSerial.h>

void setup()
{
    BluetoothSetup();
    //Bluetooth.begin(9600); // Change according to baud changes
    Serial.begin(9600);
    
    Bluetooth.print("AT");   // Expect OK answer during first loop
    delay(1000);
}

void loop()
{
    while (Bluetooth.available()){
        Serial.print((char) Bluetooth.read());
    }

    while (Serial.available()){
        Bluetooth.print((char) Serial.read());
    }
}
