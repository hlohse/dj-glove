#ifndef DJ_GLOVE_ARDUINO_DJ_GLOVE_BUTTON_H_
#define DJ_GLOVE_ARDUINO_DJ_GLOVE_BUTTON_H_

class Button {
public:
  Button(const byte pin)
  : m_pin(pin),
    m_is_pushed(false),
    m_num_debounces(0)
  {
    pinMode(m_pin, INPUT);
  }
  
  bool isPushed()
  {
    const bool is_pushed = digitalRead(m_pin) == HIGH;
    
    if (is_pushed != m_is_pushed) {
      m_num_debounces++;
    }
    else if (m_num_debounces > 0) {
      m_num_debounces--;
    }
    
    if (m_num_debounces >= necessary_debounces) {
      m_num_debounces = 0;
      m_is_pushed = is_pushed;
    }
    
    return m_is_pushed;
  }

private:
  static const int necessary_debounces = 5;
  
  byte m_pin;
  bool m_is_pushed;
  int  m_num_debounces;
};

#endif // DJ_GLOVE_ARDUINO_DJ_GLOVE_BUTTON_H_

