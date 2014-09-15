#include <Bluetooth.h>
#include <SoftwareSerial.h>
#include "Pins.h"
#include "DjGlove.h"
#include "Data.h"
 
DjGlove   glove; // Only instantiation!
Data      data(glove);
//Bluetooth bluetooth(Pins::bt_rx, Pins::bt_tx);

void setup()  
{
  analogReference(EXTERNAL);
  Serial.begin(115200);
}

void loop()
{
  for (int i = 0; i < 11; ++i) {
    Serial.println(data.nextByte(), BIN);
  }
  
  Serial.println("=====");
  delay(300);
  
/*
  const byte next_data_byte = data.nextByte() + 1; // Avoid null byte
  bluetooth.write(next_data_byte);
*/
}

