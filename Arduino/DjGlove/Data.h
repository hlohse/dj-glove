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
  Data(DjGlove& glove)
  : m_glove(&glove),
    m_index(0)
  {
  }
  
  byte nextByte()
  {
    byte result;
    
    switch (m_index) {
      case  0: result = touch();
      case  1: result = poti(0);
      case  2: result = poti(1);
      case  3: result = poti(2);
      case  4: result = flex();
      case  5: result = distance();
      case  6: result = orientation(0);
      case  7: result = orientation(1);
      case  8: result = orientation(2);
      case  9: result = pushFirstChannel();
      case 10: result = flipPushSecondProgram();
      default: result = 0;
    }
    
    m_index++;
    if (m_index > 10) {
      m_index = 0;
    }
    
    return result;
  }

private:
  DjGlove* m_glove;
  int      m_index;
  
  byte touch()
  {
    return 0;
  }
  
  byte poti(const int index)
  {
    switch (index) {
      case 0:  return m_glove->poti_0.read() & 0x7F; // 0111 1111
      default: return 0;
    }
  }
  
  byte flex()
  {
    return 0;
  }
  
  byte distance()
  {
    return 0;
  }
  
  byte orientation(const int index)
  {
    return 0;
  }
  
  byte pushFirstChannel()
  {
    byte result = 0;
    result |= (m_glove->push_0.isPushed() ? 1 : 0) << 6; // 0X00 0000
    result |= m_glove->channel & 0xF;                    // 0000 1111
    return result;
  }
  
  byte flipPushSecondProgram()
  {
    byte result = 0;
    result |= m_glove->program & 0xF; // 0000 1111
    return result;
  }
};

#endif // DJ_GLOVE_ARDUINO_DJ_GLOVE_DATA_H_

