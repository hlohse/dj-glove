#include <SoftwareSerial.h>
#include <Wire.h>
#include "DjGlove.h"
#include "Protocol.h"

const char sync_signal = '!';

DjGlove* glove = DjGlove::instance();
Protocol protocol(glove);

// #define BTCONNECTED

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
  #ifdef BTCONNECTED
  sync();
  #endif
}

void loop()
{
  #ifdef BTCONNECTED
  Serial.print(protocol.nextByte());
  #else

  
  //CHECKE SCHLAGSENSOR:
  /*int byteToSend = data.nextByte(); 
  if(byteToSend & 0x80) Serial.println(byteToSend, BIN);*/
  
  //CHECKE REST:
  Serial.println("=====");
  for(int i=0; i<11; i++) Serial.println(protocol.nextByte());
  delay(100);
  
  #endif

}

