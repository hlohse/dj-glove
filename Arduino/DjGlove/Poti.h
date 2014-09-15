#ifndef DJ_GLOVE_ARDUINO_DJ_GLOVE_POTI_H_
#define DJ_GLOVE_ARDUINO_DJ_GLOVE_POTI_H_

class Poti {
public:
  Poti(const byte pin)
  : m_pin(pin)
  {
    pinMode(m_pin, INPUT);
  }
  
  int read() const
  {
    // Adjust 0..1023 to 0..127
    return analogRead(m_pin) / 8;
  }

private:
  byte m_pin;
};

#endif // DJ_GLOVE_ARDUINO_DJ_GLOVE_POTI_H_

