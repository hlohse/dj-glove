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
const int   num_delays_ms     = 7;                                              
const int   num_message_sizes = 6;     

const int   delays_ms[num_delays_ms]         = {0, 1, 2, 4, 8, 16, 32};         
const int   message_sizes[num_message_sizes] = {1, 2, 4, 8, 16, 32};

const float warmup_factor     = 1.1;
const int   tries_per_message = 100 * warmup_factor;
const char  message_char      = 'M';
const char  start_signal      = '!';

void setup()  
{
#ifdef USE_SOFTWARE_SERIAL
  bluetooth.begin(9600);
#else
  Serial.begin(115200);
#endif
}

void setMessage(char* message, const int characters)
{
  memset(message, message_char, characters);
  message[characters] = '\0';
}

void syncStart()
{
  while (true) {
    while (!SERIAL.available());
    if (SERIAL.read() == start_signal) {
      return;
    }
  }
}

void sendMessage(char* message)
{
  for (int i = 0; i < tries_per_message; ++i) {
    SERIAL.write(message);
  }
}

void sendMessageDelayed(char* message, const int delay_ms)
{
  for (int i = 0; i < tries_per_message; ++i) {
    SERIAL.write(message);
    delay(delay_ms);
  }
}

void loop()
{
  for (int d = 0; d < num_delays_ms; ++d) {
    for (int m = 0; m < num_message_sizes; ++m) {
      const int delay_ms     = delays_ms[d];
      const int message_size = message_sizes[m];
      char message[message_size + 1];
      setMessage(message, message_size);
      
      syncStart();
      
      if (delay_ms > 0) {
        sendMessageDelayed(message, delay_ms);
      }
      else {
        sendMessage(message);
      }
    }
  }
}

