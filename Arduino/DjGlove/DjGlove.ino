#include <SoftwareSerial.h>
#include <Wire.h>
#include "DjGlove.h"
#include "Data.h"

DjGlove* glove = DjGlove::instance();
Data data(glove);

void setup()  
{
  analogReference(EXTERNAL);
  glove->initialize();
  Serial.begin(115200);
}

void loop()
{
  for (int i = 0; i < 11; ++i) {
    Serial.write(data.nextByte());
  }
  
  delay(400);
}

