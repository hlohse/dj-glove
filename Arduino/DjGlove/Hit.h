#ifndef DJ_GLOVE_ARDUINO_DJ_GLOVE_HIT_H_
#define DJ_GLOVE_ARDUINO_DJ_GLOVE_HIT_H_

#include "Acceleration.h"
#include "RingFifo.h"

class Hit {
public:
  Hit(Acceleration& acc)
  : m_acc(&acc),
    m_last_occurence_time(0)
  {
  }
  
  bool occured()
  {
    if(m_acc->hasAvailable()){
      const unsigned long dead_time = millis() - m_last_occurence_time;
    }
    if (dead_time >= necessary_dead_time) {
      const int acc_y = m_acc->read().y;
      m_buffer.insert(acc_y);
      
      if (acc_y >= threshold && m_buffer.isFull()) {
        m_last_occurence_time = millis();
        return true;
      }
    }
    
    return false;
  }
  
  int intensity() const
  {
    int intensity = m_buffer.getOrdered(3) - m_buffer.getOrdered(1)
                  + m_buffer.getOrdered(2) - m_buffer.getOrdered(0);
                  
    intensity = (intensity-500)/50;
    
    if (intensity > 126) {
      intensity = 126;
    }
    else if (intensity < 0) {
      intensity = 0;
    }
    
    return intensity;
  }

private:
  static const unsigned long necessary_dead_time = 75;
  static const int threshold = 3000;

  Acceleration*    m_acc;
  RingFifo<int, 4> m_buffer;
  unsigned long    m_last_occurence_time;
};

#endif // DJ_GLOVE_ARDUINO_DJ_GLOVE_HIT_H_

