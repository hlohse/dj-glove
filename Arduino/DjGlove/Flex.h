#ifndef DJ_GLOVE_ARDUINO_DJ_GLOVE_FLEX_H_
#define DJ_GLOVE_ARDUINO_DJ_GLOVE_FLEX_H_

class Flex {
public:
  Flex(const byte pin)
  : m_pin(pin)
  {
    pinMode(m_pin, INPUT);
  }
  
  // TODO: mean?
  int read() const
  {
    // Adjust 0..1023 to 0..127
    return analogRead(m_pin) / 8;
  }

private:
  byte m_pin;
};

#endif // DJ_GLOVE_ARDUINO_DJ_GLOVE_FLEX_H_

