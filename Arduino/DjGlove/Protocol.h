/*      Protocol Data Bytes Layout
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
 *   INT | 1ppp pppp    p: Punch intensity
 */

#ifndef DJ_GLOVE_ARDUINO_DJ_GLOVE_PROTOCOL_H_
#define DJ_GLOVE_ARDUINO_DJ_GLOVE_PROTOCOL_H_
 
#include "DjGlove.h"

class Protocol {
public:
  Protocol(DjGlove* glove)
  : m_glove(glove),
    m_index(-1),
    m_last_gyro_readout()
  {
    memset(&m_last_gyro_readout, 0, sizeof(m_last_gyro_readout));
  }
  
  byte nextByte()
  {
    byte result;
    
    m_glove->flex.update();
    
    if (m_glove->hit.occured()) {
      result = hitIntensity();
    }
    else {
      result = nextSensorReadout();
    }
    
    return result + 1; // No null byte allowed for Bt transmission
  }

private:
  DjGlove*      m_glove;
  int           m_index;
  Gyro::Readout m_last_gyro_readout;
  
  byte hitIntensity()
  {
    const byte intensity = m_glove->hit.intensity() & 0x7E; // 0111 1110
    return intensity | 0x80; // 1000 0000
  }
  
  byte nextSensorReadout()
  {
    m_index++;
    
    if (m_index > 10) {
      m_index = 0;
    }
    
    switch (m_index) {
      case  0: return touch();
      case  1: return poti(m_glove->poti_0);
      case  2: return poti(m_glove->poti_1);
      case  3: return poti(m_glove->poti_2);
      case  4: return flex();
      case  5: return distance();
      case  6: return orientation(0);
      case  7: return orientation(1);
      case  8: return orientation(2);
      case  9: return pushFirstChannel();
      case 10: return flipPushSecondProgram();
      default: return 0;
    }
  }
  
  byte buttonBit(Button& button, const int index)
  {
    return (button.isPressed() ? 1 : 0) << index;
  }
  
  byte touch()
  {
    byte result = 0;
    result |= buttonBit(m_glove->touch_0, 0); // 0000 000X
    result |= buttonBit(m_glove->touch_1, 1); // 0000 00X0
    result |= buttonBit(m_glove->touch_2, 2); // 0000 0X00
    result |= buttonBit(m_glove->touch_3, 3); // 0000 X000
    return result & 0xF; // 0000 1111
  }
  
  byte poti(const Poti& poti)
  {
    return poti.read() & 0x7F; // 0111 1111
  }
  
  byte flex()
  {
    return (m_glove->flex.read() / 8) & 0x7F; // 0111 1111
  }
  
  byte distance()
  {
    return m_glove->ultra_sound.read() & 0x7F; // 0111 1111
  }
  
  byte orientation(const int index)
  {
    int value = 0;
    
    switch (index) {
      case 0:
        m_last_gyro_readout = m_glove->gyro.read();
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
    byte result = 0;
    result |= m_glove->channel & 0xF;        // 0000 1111
    result |= buttonBit(m_glove->push_2, 4); // 000X 0000
    result |= buttonBit(m_glove->push_3, 5); // 00X0 0000
    result |= buttonBit(m_glove->push_4, 6); // 0X00 0000
    return result & 0x7F; // 0111 1111
  }
  
  byte flipPushSecondProgram()
  {
    byte result = 0;
    result |= m_glove->program & 0xF;        // 0000 1111
    result |= buttonBit(m_glove->flip,   6); // 0X00 0000
    result |= buttonBit(m_glove->push_0, 5); // 00X0 0000
    result |= buttonBit(m_glove->push_1, 4); // 000X 0000
    return result & 0x7F; // 0111 1111
  }
};

#endif // DJ_GLOVE_ARDUINO_DJ_GLOVE_PROTOCOL_H_

