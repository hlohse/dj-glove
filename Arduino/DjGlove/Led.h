#ifndef DJ_GLOVE_ARDUINO_DJ_GLOVE_LED_H_
#define DJ_GLOVE_ARDUINO_DJ_GLOVE_LED_H_

class Led {
public:
  Led(const byte pin_clk, const byte pin_sda)
  : m_pin_clk(pin_clk),
    m_pin_sda(pin_sda),
    m_left(0),
    m_right(0)
  {
    pinMode(m_pin_clk, OUTPUT);
    pinMode(m_pin_sda, OUTPUT);
  }
  
  void display() const
  {
    const digit_t& digit_left  = Digit[digitIndex(left())];
    const digit_t& digit_right = Digit[digitIndex(right())];
    
    write(digit_right, 0, 4);
    write(digit_left,  0, 4);
    write(digit_left,  4, 4);
    write(digit_right, 4, 4);
  }
  
  // setLeft(0); ... setLeft(9); setLeft('C'); setLeft('P');
  void setLeft(const unsigned char value)
  {
    set(m_left, value);
  }
  
  // setRight(0); ... setRight(9); setRight('C'); setRight('P');
  void setRight(const unsigned char value)
  {
    set(m_right, value);
  }
  
  unsigned char left() const
  {
    return m_left;
  }
  
  unsigned char right() const
  {
    return m_right;
  }

private:
  static const int digit_size = 8;
  static const int num_digits = 13;
  typedef unsigned char digit_t[digit_size];
  static const digit_t Digit[num_digits];
  
  byte          m_pin_clk;
  byte          m_pin_sda;
  unsigned char m_left;
  unsigned char m_right;
  
  void write(const digit_t& led, const int offset, const int num) const
  {
    for (int i = offset; i < offset + num; ++i) {
      write(led[i]);
    }
  }
  
  void write(const unsigned char value) const
  {
      digitalWrite(m_pin_sda, value);
      digitalWrite(m_pin_clk, HIGH);
      digitalWrite(m_pin_clk, LOW);
  }
  
  int digitIndex(const unsigned char value) const
  {
    if (value <= 9) {
      return value;
    }
    
    switch (value) {
      case 'C': return 10; break;
      case 'P': return 11; break;
      default:  return 12; break; // 'E'
    }
  }
  
  void set(unsigned char& side, const unsigned char value)
  {
    const unsigned char digit_index = digitIndex(value);
    
    if (digit_index <= 11) {
      side = value;
    }
    else {
      side = 'E';
    }
  }
};
  
const Led::digit_t Led::Digit[Led::num_digits] = {
  {HIGH, HIGH, HIGH,  LOW, HIGH, HIGH, HIGH, LOW}, // 0, index 0
  {HIGH,  LOW,  LOW,  LOW,  LOW,  LOW, HIGH, LOW}, // 1
  {HIGH, HIGH,  LOW, HIGH, HIGH, HIGH,  LOW, LOW}, // 2
  {HIGH, HIGH,  LOW, HIGH,  LOW, HIGH, HIGH, LOW}, // 3
  {HIGH,  LOW, HIGH, HIGH,  LOW,  LOW, HIGH, LOW}, // 4
  { LOW, HIGH, HIGH, HIGH,  LOW, HIGH, HIGH, LOW}, // 5
  { LOW, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, LOW}, // 6
  {HIGH, HIGH,  LOW,  LOW,  LOW,  LOW, HIGH, LOW}, // 7
  {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, LOW}, // 8
  {HIGH, HIGH, HIGH, HIGH,  LOW, HIGH, HIGH, LOW}, // 9, index 9
  { LOW, HIGH, HIGH,  LOW, HIGH, HIGH,  LOW, LOW}, // C, index 10
  {HIGH, HIGH, HIGH, HIGH, HIGH,  LOW,  LOW, LOW}, // P, index 11
  { LOW, HIGH, HIGH, HIGH, HIGH, HIGH,  LOW, LOW}  // E, index 12 TODO
};

#endif // DJ_GLOVE_ARDUINO_DJ_GLOVE_LED_H_

