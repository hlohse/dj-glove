#include <SoftwareSerial.h>
#include <Wire.h>
#include "DjGlove.h"
#include "Protocol.h"

const char sync_signal = '!';

DjGlove* glove = DjGlove::instance();
Protocol protocol(glove);

void sync()
{
  while (true) {
    while (!Serial.available());
    
    if (Serial.read() == sync_signal) {
      return;
    }
  }
}

void setup()  
{
  glove->initialize();
  Serial.begin(115200);
  sync();
}

void loop()
{
  Serial.write(protocol.nextByte());
}

