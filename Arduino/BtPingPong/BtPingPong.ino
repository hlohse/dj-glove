#include <Bluetooth.h>
#include <SoftwareSerial.h>

void setup()  
{
    BluetoothSetup();
}

void loop()
{
    Bluetooth.println("Bluetooth test!");
    
    delay(100);
}

