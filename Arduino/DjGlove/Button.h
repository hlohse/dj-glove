#ifndef DJ_GLOVE_ARDUINO_DJ_GLOVE_BUTTON_H_
#define DJ_GLOVE_ARDUINO_DJ_GLOVE_BUTTON_H_

class Button {
public:
  Button(const byte pin)
  : m_pin(pin),
    m_is_pressed(false),
    m_num_debounces(0),
    m_press_handler(NULL),
    m_release_handler(NULL)
  {
    pinMode(m_pin, INPUT);
  }
  
  bool isPressed()
  {
    const bool is_pressed = digitalRead(m_pin) == HIGH;
    
    debounce(is_pressed);
    
    if (isDebounced()) {
      updateIsPressed(is_pressed);
      notifyHandler();
    }
    
    return m_is_pressed;
  }
  
  void assignOnPress(void (*handler)())
  {
    m_press_handler = handler;
  }
  
  void assignOnRelease(void (*handler)())
  {
    m_release_handler = handler;
  }

private:
  static const int necessary_debounces = 5;
  
  byte m_pin;
  bool m_is_pressed;
  int  m_num_debounces;
  void (*m_press_handler)();
  void (*m_release_handler)();
  
  void debounce(const bool is_pressed)
  {
    if (is_pressed != m_is_pressed) {
      m_num_debounces++;
    }
    else if (m_num_debounces > 0) {
      m_num_debounces--;
    }
  }
  
  bool isDebounced()
  {
    return m_num_debounces >= necessary_debounces;
  }
  
  void updateIsPressed(const bool is_pressed)
  {
      m_num_debounces = 0;
      m_is_pressed = is_pressed;
  }
  
  void notifyHandler()
  {
    void (*handler)() = m_is_pressed ? m_press_handler : m_release_handler;
    
    if (handler != NULL) {
      handler();
    }
  }
};

#endif // DJ_GLOVE_ARDUINO_DJ_GLOVE_BUTTON_H_

