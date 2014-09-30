#ifndef DJ_GLOVE_ARDUINO_DJ_GLOVE_FLEX_H_
#define DJ_GLOVE_ARDUINO_DJ_GLOVE_FLEX_H_

#include "RingFifo.h"

class Flex {
public:
  Flex(const byte pin)
  : m_pin(pin)
  {
    pinMode(m_pin, INPUT);
  }
  
  void update()
  {
    m_buffer.insert(analogRead(m_pin));
  }
  
  int read() const
  {
    int sum = 0;
    
    if (m_buffer.isEmpty()) {
      return 0;
    }
    
    for (int i = 0; i < m_buffer.count(); ++i) {
      sum += m_buffer[i];
    }
    
    return sum / m_buffer.size();
  }

private:
  byte              m_pin;
  RingFifo<int, 10> m_buffer;
};

#endif // DJ_GLOVE_ARDUINO_DJ_GLOVE_FLEX_H_

