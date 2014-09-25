#include <SoftwareSerial.h>
#include <Wire.h>
#include "DjGlove.h"
#include "Data.h"
 
DjGlove   glove; // Only instantiation!
Data      data(glove);

void setup()  
{
  analogReference(EXTERNAL);
  glove.initialize();
  Serial.begin(9600);
  Serial.println("GO!");
}

void loop()
{
  for (int i = 0; i < 11; ++i) {
    Serial.println(data.nextByte(), BIN);
  }
  
  Serial.println("=====");
  delay(300);
}

