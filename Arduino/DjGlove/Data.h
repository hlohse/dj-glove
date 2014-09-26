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
 */

#ifndef DJ_GLOVE_ARDUINO_DJ_GLOVE_DATA_H_
#define DJ_GLOVE_ARDUINO_DJ_GLOVE_DATA_H_
 
#include "DjGlove.h"

class Data {
public:
  Data()
  : m_index(0),
    m_last_gyro_readout()
  {
    memset(&m_last_gyro_readout, 0, sizeof(m_last_gyro_readout));
  }
  
  byte nextByte()
  {
    DjGlove* glove = DjGlove::instance();
    byte result;
    
    switch (m_index) {
      case  0: result = touch();                 break;
      case  1: result = poti(glove->poti_0);     break;
      case  2: result = poti(glove->poti_1);     break;
      case  3: result = poti(glove->poti_2);     break;
      case  4: result = flex();                  break;
      case  5: result = distance();              break;
      case  6: result = orientation(0);          break;
      case  7: result = orientation(1);          break;
      case  8: result = orientation(2);          break;
      case  9: result = pushFirstChannel();      break;
      case 10: result = flipPushSecondProgram(); break;
      default: result = 0;                       break;
    }
    
    m_index++;
    if (m_index > 10) {
      m_index = 0;
    }
    
    return result + 1;
  }

private:
  int           m_index;
  Gyro::Readout m_last_gyro_readout;
  
  byte buttonBit(Button& button, const int index)
  {
    return (button.isPressed() ? 1 : 0) << index;
  }
  
  byte touch()
  {
    DjGlove* glove = DjGlove::instance();
    byte result = 0;
    
    result |= buttonBit(glove->touch_0, 0); // 0000 000X
    result |= buttonBit(glove->touch_1, 1); // 0000 00X0
    result |= buttonBit(glove->touch_2, 2); // 0000 0X00
    result |= buttonBit(glove->touch_3, 3); // 0000 X000
    
    return result & 0xF; // 0000 1111
  }
  
  byte poti(const Poti& poti)
  {
    return poti.read() & 0x7F; // 0111 1111
  }
  
  byte flex()
  {
    return DjGlove::instance()->flex.read() & 0x7F; // 0111 1111
  }
  
  byte distance()
  {
    return DjGlove::instance()->ultra_sound.read() & 0x7F; // 0111 1111
  }
  
  byte orientation(const int index)
  {
    int value = 0;
    
    switch (index) {
      case 0:
        m_last_gyro_readout = DjGlove::instance()->gyro.read();
        value = m_last_gyro_readout.x;
        break; 
      case 1:
        value = m_last_gyro_readout.y;
        break;
      case 2:
        value = m_last_gyro_readout.z;
        break;
    }
    
    const byte value_as_byte = value / 0xFF;
    const byte value_7_bit   = value_as_byte / 2;
    
    return value_7_bit & 0x7F; // 0111 1111
  }
  
  byte pushFirstChannel()
  {
    DjGlove* glove = DjGlove::instance();
    byte result = 0;
    
    result |= glove->channel & 0xF;        // 0000 1111
    result |= buttonBit(glove->push_2, 4); // 000X 0000
    result |= buttonBit(glove->push_3, 5); // 00X0 0000
    result |= buttonBit(glove->push_4, 6); // 0X00 0000
    
    return result & 0x7F; // 0111 1111
  }
  
  byte flipPushSecondProgram()
  {
    DjGlove* glove = DjGlove::instance();
    byte result = 0;
    
    result |= glove->program & 0xF;        // 0000 1111
    result |= buttonBit(glove->flip,   6); // 0X00 0000
    result |= buttonBit(glove->push_0, 5); // 00X0 0000
    result |= buttonBit(glove->push_1, 4); // 000X 0000
    
    return result & 0x7F; // 0111 1111
  }
};

#endif // DJ_GLOVE_ARDUINO_DJ_GLOVE_DATA_H_

