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
    for (int i = 0; i < num_update_readouts; ++i) {
      m_buffer.insert(analogRead(m_pin));
    }
  }
  
  int read() const
  {
    int sum = 0;
    
    if (m_buffer.isEmpty()) {
      return 0;
    }
    
    for (int i = 0; i < m_buffer.count(); ++i) {
      sum += m_buffer.getDirectly(i);
    }
    
    const int mean = sum / m_buffer.count();
    const float range_adjusted = ((float) (mean - readout_min)) * range_factor;
    
    if (range_adjusted < 0) {
      return 0;
    }
    else if (range_adjusted > 127) {
      return 127;
    }
    else {
      return range_adjusted;
    }
  }

private:
  static const int   num_update_readouts = 3;
  static const int   buffer_size = 11 * num_update_readouts; // For each protocol byte
  static const int   readout_min = 250;
  static const int   readout_max = 560;
  static const float range_factor = 127.0 / (readout_max - readout_min);
  
  byte m_pin;
  RingFifo<int, buffer_size> m_buffer;
};

#endif // DJ_GLOVE_ARDUINO_DJ_GLOVE_FLEX_H_

