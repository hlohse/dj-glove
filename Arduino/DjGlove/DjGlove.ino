#include <Bluetooth.h>
#include <SoftwareSerial.h>

struct Pins {
  static const byte led_clk = A3;
  static const byte led_sda = A2;
};

class Led {
public:
  Led(const byte pin_clk, const byte pin_sda)
  :   m_pin_clk(pin_clk),
      m_pin_sda(pin_sda)
  {
    pinMode(m_pin_clk, OUTPUT);
    pinMode(m_pin_sda, OUTPUT);
    setLeft(0);
    setRight(0);
  }
  
  ~Led()
  {
  }
  
  void display() const
  {
    write(m_left_led,  0, 4);
    write(m_right_led, 0, 4);
    write(m_left_led,  4, 4);
    write(m_right_led, 4, 4);
  }
  
  void setLeft(const unsigned char value)
  {
    set(m_left, m_left_led, value);
  }
  
  void setRight(const unsigned char value)
  {
    set(m_right, m_right_led, value);
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
  digit_t       m_left_led;
  digit_t       m_right_led;
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
  
  int digitIndex(const unsigned char value)
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
  
  void set(unsigned char& side, digit_t& led, const unsigned char value)
  {
    const unsigned char digit_index = digitIndex(value);
    
    if (digit_index != 9) { // E
      side = value;
    }
    else {
      side = 'E';
    }
    
    memcpy(&led, &Digit[digit_index], sizeof(led));
  }
};
  
const Led::digit_t Led::Digit[Led::num_digits] = {
  {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, LOW}, // 0, index 0
  {HIGH, HIGH,  LOW,  LOW,  LOW,  LOW,  LOW, LOW}, // 1
  {HIGH, HIGH,  LOW, HIGH, HIGH, HIGH,  LOW, LOW}, // 2
  {HIGH, HIGH,  LOW, HIGH,  LOW, HIGH, HIGH, LOW}, // 3
  {HIGH,  LOW, HIGH, HIGH,  LOW,  LOW, HIGH, LOW}, // 4
  { LOW, HIGH, HIGH, HIGH,  LOW, HIGH,  LOW, LOW}, // 5
  { LOW, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, LOW}, // 6
  {HIGH, HIGH,  LOW,  LOW,  LOW,  LOW, HIGH, LOW}, // 7
  {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, LOW}, // 8
  {HIGH, HIGH, HIGH, HIGH,  LOW, HIGH, HIGH, LOW}, // 9, index 9
  {HIGH, HIGH, HIGH, HIGH, HIGH,  LOW,  LOW, LOW}, // C, index 10
  { LOW, HIGH, HIGH,  LOW, HIGH, HIGH,  LOW, LOW}, // P, index 11
  { LOW,  LOW, HIGH,  LOW,  LOW,  LOW,  LOW, LOW}  // E, index 12 TODO
};

void setup()  
{
  BluetoothSetup();
}

void loop()
{
  
}
