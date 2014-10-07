#include <SoftwareSerial.h>
#include <Wire.h>
#include "DjGlove.h"
#include "Protocol.h"

const char sync_signal = '!';

DjGlove* glove = DjGlove::instance();
Protocol protocol(glove);

#define BTCONNECTED 1

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
  Serial.write(protocol.nextByte());
  //delay(100);
  #else
  
  /*
  int byteToSend = protocol.nextByte();
  if(byteToSend & 0x80) {
    Serial.print(glove->hit.m_buffer.getOrdered(0));
    Serial.print("\t");
    Serial.print(glove->hit.m_buffer.getOrdered(1));
    Serial.print("\t");
    Serial.print(glove->hit.m_buffer.getOrdered(2));
    Serial.print("\t");
    Serial.println(glove->hit.m_buffer.getOrdered(3));
  }
  */
  
  
  
  //CHECKE SCHLAGSENSOR:
  //int byteToSend = protocol.nextByte(); 
  //if(byteToSend & 0x80) Serial.println(byteToSend & B01111111);
  
  //CHECKE REST:
  
  //Serial.println("=====");
    //Serial.write(protocol.nextByte());
  //delay(200);
  
  /*
    const int x = glove->gyro.read().x;
    const int y = glove->gyro.read().y;
    Serial.print(x);
    Serial.print("\t");
    Serial.print(x, BIN);
    Serial.print("\t");
    Serial.print(y);
    Serial.print("\t");
    Serial.println(y, BIN);
    */
  //delay(200);
  
  
  /*
  const Gyro::Readout readout = glove->gyro.read();
  readout.x /= 2;
  readout.y /= 2;
  
  readout.x 
  
  
  Serial.print(readout.x / 2);
  Serial.print("\t");
  Serial.println(readout.y / 2);
  */
  
  
  //delay(200);
  
  #endif

}

