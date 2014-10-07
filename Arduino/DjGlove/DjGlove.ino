
#define BTCONNECTED 1 // Comment out if using USB

#include <SoftwareSerial.h>
#include <Wire.h>
#include "DjGlove.h"
#include "Protocol.h"

const char* sync_signal = "!";

DjGlove* glove = DjGlove::instance();
Protocol protocol(glove);

void sync()
{
  while (true) {
    while (!Serial.available());
    
    if (Serial.read() == sync_signal[0]) {
      Serial.write(sync_signal);
      return;
    }
  }
}

void setup()  
{
  glove->initialize();
  Serial.begin(115200);
  
  #ifdef BTCONNECTED
  sync();
  #endif
}

void loop()
{
  #ifdef BTCONNECTED
  
  Serial.write(protocol.nextByte());
  
  #else
  
  for (int i = 0; i < 12; ++i) {
    Serial.println(protocol.nextByte(), BIN);
  }
  
  Serial.println("===");
  delay(200);
  
  #endif
}

