#include <Bluetooth.h>
#include <SoftwareSerial.h>

Bluetooth bluetooth;

void setup()
{
    // bluetooth.begin(9600); // Change according to baud changes
    Serial.begin(9600);
    
    bluetooth.print("AT");   // Expect OK answer during first loop
    delay(1000);
}

void loop()
{
    while (bluetooth.available()){
        Serial.print((char) bluetooth.read());
    }

    while (Serial.available()){
        bluetooth.print((char) Serial.read());
    }
}
