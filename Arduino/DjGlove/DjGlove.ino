#include <Bluetooth.h>
#include <SoftwareSerial.h>
#include "Pins.h"
#include "DjGlove.h"
#include "Data.h"
 
DjGlove   glove; // Only instantiation!
Data      data(glove);
Bluetooth bluetooth(Pins::bt_rx, Pins::bt_tx);

void setup()  
{
  Serial.begin(115200);
}

void toBinaryString(const byte value, char* binary_string)
{
  for (int i = 0; i < 8; ++i) {
    binary_string[i] = value >= (1 << (8 - (i + 1))) ? '1' : '0';
  }
  
  binary_string[8] = '\0';
}

void loop()
{
  char binary_string[9];
  
  for (int i = 0; i < 11; ++i) {
    toBinaryString(data.nextByte(), binary_string);
    Serial.println(binary_string);
  }
  
  Serial.println("=====");
  delay(1000);
  
/*
  const byte next_data_byte = data.nextByte() + 1; // Avoid null byte
  bluetooth.write(next_data_byte);
*/
}

