#include <Arduino.h>
#include <SoftwareSerial.h>

// Comment out to use hardware Serial pins RX0, TX1
#define USE_SOFTWARE_SERIAL 1

#ifdef USE_SOFTWARE_SERIAL
SoftwareSerial bluetooth(2, 3);
#define _SERIAL_ bluetooth
#else
#define _SERIAL_ Serial
#endif

// NOTE: USE THIS VALUES IN PC PROJECT, TOO!
const unsigned int num_messages = 100;
const char*        message      = "PING";
const char         start_signal = '!';

void setup()  
{
#ifndef USE_SOFTWARE_SERIAL
    Serial.begin(115200);
#endif
}

void loop()
{
    if (_SERIAL_.available()) {
        if (_SERIAL_.read() == start_signal) {
            for (unsigned int i = 0; i < num_messages; ++i) {
                _SERIAL_.print(message);
            }
        }
    }
}

