#include <Bluetooth.h>
#include <SoftwareSerial.h>
#include <Wire.h>
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
  Serial.println(glove.ultra_sound.read());
  delay(500);
  
/*
  for (unsigned char i = 0; i < 10; ++i) {
    glove.led.setLeft(0);
    glove.led.setRight(i);
    glove.led.display();
    delay(1000);
  }
*/
  
/*
  glove.led.setLeft('P');
  glove.led.setRight('E');
  glove.led.display();
  delay(1000);
  
  glove.led.setLeft('E');
  glove.led.setRight('P');
  glove.led.display();
  delay(1000);
*/
  
/*
  for (int i = 0; i < 11; ++i) {
    Serial.println(data.nextByte(), BIN);
  }
  
  Serial.println("=====");
  delay(300);
*/
  
/*
  const byte next_data_byte = data.nextByte() + 1; // Avoid null byte
  bluetooth.write(next_data_byte);
*/
}

