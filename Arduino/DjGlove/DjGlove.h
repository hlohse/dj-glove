#ifndef DJ_GLOVE_ARDUINO_DJ_GLOVE_DJ_GLOVE_H_
#define DJ_GLOVE_ARDUINO_DJ_GLOVE_DJ_GLOVE_H_

#include "Config.h"
#include "Button.h"
#include "Poti.h"
#include "Flex.h"
#include "UltraSound.h"
#include "Gyro.h"
#include "Acceleration.h"
#include "Hit.h"
#include "Led.h"

// Note: Singleton. Access pointer to instance via DjGlove::instance()
class DjGlove {
public:
  Button       push_0;
  Button       push_1;
  Button       push_2;
  Button       push_3;
  Button       push_4;
  Button       touch_0;
  Button       touch_1;
  Button       touch_2;
  Button       touch_3;
  Button       flip;
  Poti         poti_0;
  Poti         poti_1;
  Poti         poti_2;
  Flex         flex;
  UltraSound   ultra_sound;
  Gyro         gyro;
  Acceleration acc;
  Hit          hit;
  Led          led;
  byte         channel;
  byte         program;
  
  static DjGlove* instance()
  {
    static DjGlove instance;
    return &instance;
  }
  
  // Call once first!
  void initialize()
  {
    analogReference(EXTERNAL);
    Wire.begin();
    
    ultra_sound.initialize();
    gyro.initialize();
    acc.initialize();
    
    presentLed();
    push_0.assignOnPress(&DjGlove::nextProgram);
    push_1.assignOnPress(&DjGlove::nextChannel);
  }

private:
  DjGlove()
  : push_0(Pin::push_0),
    push_1(Pin::push_1),
    push_2(Pin::push_2),
    push_3(Pin::push_3),
    push_4(Pin::push_4),
    touch_0(Pin::touch_0),
    touch_1(Pin::touch_1),
    touch_2(Pin::touch_2),
    touch_3(Pin::touch_3),
    flip(Pin::flip),
    poti_0(Pin::poti_0),
    poti_1(Pin::poti_1),
    poti_2(Pin::poti_2),
    flex(Pin::flex),
    ultra_sound(Address::ultra_sound, UltraSound::UNIT_MS),
    gyro(Address::gyro),
    acc(Address::acc, Pin::acc_int),
    hit(acc),
    led(Pin::led_clk, Pin::led_sda),
    channel(1),
    program(1)
  {
  }
  
  void presentLed()
  {
    led.setLeft('C');
    led.setRight('P');
    led.display();
    
    delay(1000);
    
    led.setLeft(channel);
    led.setRight(program);
    led.display();
  }
  
  static void nextChannel()
  {
    next(DjGlove::instance()->channel, &Led::setLeft);
  }
  
  static void nextProgram()
  {
    next(DjGlove::instance()->program, &Led::setRight);
  }
  
  static void next(byte& value, void (Led::*setter)(const unsigned char))
  {
    byte display_value;
    
    value++;
    if (value >= 11) {
      value = 1;
    }
    
    display_value = value == 10 ? 0 : value;
    
    (DjGlove::instance()->led.*setter)(display_value);
    DjGlove::instance()->led.display();
  }
};

#endif // DJ_GLOVE_ARDUINO_DJ_GLOVE_DJ_GLOVE_H_

