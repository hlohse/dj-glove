#include <Arduino.h>
#include <SoftwareSerial.h>

// Comment out to use hardware Serial pins RX0, TX1
//#define USE_SOFTWARE_SERIAL 1

#ifdef USE_SOFTWARE_SERIAL
SoftwareSerial bluetooth(2, 3);
#define SERIAL bluetooth
#else
#define SERIAL Serial
#endif

// NOTE: USE THIS VALUES IN PC PROJECT, TOO!
const int   iterations = 1001;
const char  message    = '!';

void setup()  
{
#ifdef USE_SOFTWARE_SERIAL
  bluetooth.begin(9600);
#else
  Serial.begin(115200);
#endif
}

void loop()
{
  for (int i = 0; i < iterations; ++i) {
    while (!Serial.available());
    if (Serial.read() == message) {
      Serial.write(message);
    }
  }
}

