#ifndef DJ_GLOVE_ARDUINO_DJ_GLOVE_HIT_H_
#define DJ_GLOVE_ARDUINO_DJ_GLOVE_HIT_H_

#include "Acceleration.h"

class Hit {
public:
  Hit(Acceleration& acc)
  : m_acc(&acc),
    m_index(0),
    m_last_occurence_time(0)
  {
    memset(m_buffer, 0, num_values);
  }
  
  bool occured()
  {
    const unsigned long dead_time = millis() - m_last_occurence_time;
    
    if (dead_time >= necessary_dead_time && m_acc->hasAvailable()) {
      m_index++;
      if (m_index >= num_values) {
        m_index = 0;
      }
      
      m_buffer[m_index] = m_acc->read().y;
      
      if (m_buffer[m_index] >= threshold) {
        m_last_occurence_time = millis();
        return true;
      }
    }
    
    return false;
  }
  
  int intensity() const
  {
    int intensity = m_buffer[m_index]       - m_buffer[(m_index+2)%4]
                  + m_buffer[(m_index+3)%4] - m_buffer[(m_index+1)%4];
                  
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
  static const int num_values = 4;
  static const int threshold  = 3000;
  static const unsigned long necessary_dead_time = 30;

  Acceleration* m_acc;
  int           m_buffer[num_values];
  int           m_index;
  unsigned long m_last_occurence_time;
};

#endif // DJ_GLOVE_ARDUINO_DJ_GLOVE_HIT_H_

