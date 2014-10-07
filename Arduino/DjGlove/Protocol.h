/*      Protocol Data Bytes Layout
 *
 * Index | Bits         Signals
 * ----------------------------------------------------------------------
 *     0 | 0000 tttt    t: Touch buttons 0..3
 *     1 | 0ppp pppp    p: Poti 0 [7]
 *     2 | 0ppp pppp    p: Poti 1 [7]
 *     3 | 0ppp pppp    p: Poti 2 [7]
 *     4 | 0fff ffff    f: Flex [7]
 *     5 | 0ddd dddd    d: Distance [7]
 *     6 | 0ooo oooo    o: Orientation X low  [7]
 *     7 | 0ooo oooo    o: Orientation X high [7]
 *     8 | 0ooo oooo    o: Orientation Y low  [7]
 *     9 | 0ooo oooo    o: Orientation Y high [7]
 *    10 | 0bbb cccc    b: Push buttons 0..2, c: Channel [4]
 *    11 | 0fbb pppp    f: Flip button, b: Push buttons 3..4, p: Program [4]
 *   INT | 1ppp pppp    p: Punch intensity [7]
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
    #ifdef BTCONNECTED
    return result + 1; // No null byte allowed for Bt transmission
    #else
    return result;
    #endif
  }
  
  int getByteIndex(){
    return m_index;
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
    
    if (m_index > 11) {
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
      case  9: return orientation(3);
      case 10: return pushFirstChannel();
      case 11: return flipPushSecondProgram();
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
    return m_glove->flex.read() & 0x7F; // 0111 1111
  }
  
  byte distance()
  {
    return m_glove->ultra_sound.read() & 0x7F; // 0111 1111
  }
  
  int bits(const int value, const byte offset, const byte num_bits)
  {
    const int shifted = value >> offset;
    return shifted & (1 << (num_bits - 1));
  }
  
  byte orientation(const int index)
  {
    byte value = 0;
    
    switch (index) {
      case 0:
        m_last_gyro_readout    = m_glove->gyro.read();
        m_last_gyro_readout.x /= 4; // Adjust to 14 bit
        m_last_gyro_readout.y /= 4; // Adjust to 14 bit
        value = bits(m_last_gyro_readout.x, 0, 7);
        break; 
      case 1:
        value  = bits(m_last_gyro_readout.x, 7, 6);
        // Signed bit at 0SXX XXXX
        value |= m_last_gyro_readout.x < 0 ? 0x40 : 0;
        break;
      case 2:
        value = bits(m_last_gyro_readout.y, 0, 7);
        break;
      case 3:
        value = bits(m_last_gyro_readout.y, 7, 6);
        // Signed bit at 0SXX XXXX
        value |= m_last_gyro_readout.y < 0 ? 0x40 : 0;
        break;
    }
    
    return value & 0x7F; // 0111 1111
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

