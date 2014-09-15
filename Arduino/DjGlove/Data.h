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

#ifndef DJ_GLOVE_ARDUINO_DJ_GLOVE_DATA_H_
#define DJ_GLOVE_ARDUINO_DJ_GLOVE_DATA_H_
 
#include "DjGlove.h"

class Data {
public:
  byte bytes[11];
  
  Data(DjGlove& glove)
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

#endif // DJ_GLOVE_ARDUINO_DJ_GLOVE_DATA_H_

