#include <Bluetooth.h>
#include <SoftwareSerial.h>

struct Pins {
  static const byte bt_rx   =  2; // Connected to TXD
  static const byte bt_tx   =  3; // Connected to RXD
  static const byte poti_0  = A0;
  static const byte push_0  =  3;
  static const byte led_clk = A3;
  static const byte led_sda = A2;
};

/* =========================
 *  Flex
 * =========================*/
class Flex {
// TODO
};

/* =========================
 *  Poti
 * =========================*/
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

/* =========================
 *  PushButton
 * =========================*/
class PushButton {
public:
  PushButton(const byte pin)
  : m_pin(pin),
    m_num_debounces(0)
  {
    pinMode(m_pin, INPUT);
  }
  
  bool isPushed()
  {
    const bool is_pushed = digitalRead(m_pin) == HIGH;
    
    if (!is_pushed) {
      m_num_debounces = 0;
      return false;
    }
    
    if (m_num_debounces < necessary_debounces) {
      m_num_debounces++;
    }
      
    return m_num_debounces == necessary_debounces;
  }

private:
  static const int necessary_debounces = 5;
  
  byte m_pin;
  int  m_num_debounces;
};

/* =========================
 *  Led
 * =========================*/
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
    
    write(digit_left,  0, 4);
    write(digit_right, 0, 4);
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

/* =========================
 *  DjGlove
 * =========================*/
 
// Note: Instantiate only once
struct DjGlove {
  Poti       poti_0;
  PushButton push_0;
  Led        led;
  byte       channel;
  byte       program;
  
  DjGlove()
  : poti_0(Pins::poti_0),
    push_0(Pins::push_0),
    led(Pins::led_clk, Pins::led_sda),
    channel(0),
    program(0)
  {
  }
};

/* =========================
 *  DjGloveData
 * =========================*/

/*      Data Bytes Layout
 *
 * Index | Bits         Signals
 * ----------------------------------------------------------------------
 *     0 | 0000 tttt    t: Touch buttons 0..3
 *     1 | 0ppp pppp    p: Poti 0
 *     2 | 0ppp pppp    p: Poti 1
 *     3 | 0ppp pppp    p: Poti 2
 *     4 | 0fff ffff    f: Flex
 *     5 | 0ddd dddd    d: Distance
 *     6 | 0ooo oooo    o: Orientation X
 *     7 | 0ooo oooo    o: Orientation Y
 *     8 | 0ooo oooo    o: Orientation Z
 *     9 | 0bbb cccc    b: Push buttons 0..2, c: Channel
 *    10 | 0fbb pppp    f: Flip button, b: Push buttons 3..4, p: Program
 *   INT | 1000 000p    p: Punch
 *
 * Note: Increment each value by 1 for Bluetooth transmission (no null bytes allowed)
 */

class DjGloveData {
public:
  byte bytes[11];
  
  DjGloveData(DjGlove& glove)
  : bytes({0})
  {
    readTouchButtons(glove);
    readPotis(glove);
    readFlex(glove);
    readDistance(glove);
    readOrientation(glove);
    readPushButtons(glove);
    readChannel(glove);
    readFlipButton(glove);
    readProgram(glove);
  }

private:
  void readTouchButtons(DjGlove& glove)
  {
  }
  
  void readPotis(DjGlove& glove)
  {
    bytes[1] = glove.poti_0.read();
  }
  
  void readFlex(DjGlove& glove)
  {
  }
  
  void readDistance(DjGlove& glove)
  {
  }
  
  void readOrientation(DjGlove& glove)
  {
  }
  
  void readPushButtons(DjGlove& glove)
  {
    bytes[9] |= (glove.push_0.isPushed() ? 1 : 0) << 6;
  }
  
  void readChannel(DjGlove& glove)
  {
    bytes[9] |= glove.channel & 0xF; // 0000 1111
  }
  
  void readFlipButton(DjGlove& glove)
  {
  }
  
  void readProgram(DjGlove& glove)
  {
    bytes[10] |= glove.program & 0xF; // 0000 1111
  }
};

/* =========================
 *  main
 * =========================*/
 
DjGlove   glove; // Only instantiation!
Bluetooth bluetooth(Pins::bt_rx, Pins::bt_tx);

void setup()  
{
}

void loop()
{  
}

