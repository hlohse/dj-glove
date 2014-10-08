
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

int j = 1;

void loop()
{
  #ifdef BTCONNECTED
  
  Serial.write(protocol.nextByte());
  
  #else
  
  for (int i = 0; i < 12;) {
    const byte data = protocol.nextByte();
    
    if (data & 0x80) {
      Serial.print("HITTTTTTTTTTTTTTTTTTTTTTTTTt ");
      Serial.println(data, BIN);
    }
    else {
      i++;
      if (j >= 100) {
        Serial.println(data, BIN);
      }
    }
  }
  
  j++;
  if ( j > 100) j = 0;
  
  #endif
}

