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
 *     6 | 0ooo oooo    o: Orientation X low
 *     7 | 0ooo oooo    o: Orientation X high
 *     8 | 0ooo oooo    o: Orientation Y low
 *     9 | 0ooo oooo    o: Orientation Y high
 *    10 | 0bbb cccc    b: Push buttons 0..2, c: Channel
 *    11 | 0fbb pppp    f: Flip button, b: Push buttons 3..4, p: Program
 *   INT | 1000 000p    p: Punch
 */

#ifndef DJ_GLOVE_PC_PROTOCOL_H_
#define DJ_GLOVE_PC_PROTOCOL_H_

#include "Export.h"

class DjGlove;

class EXPORT Protocol {
public:
    Protocol(DjGlove& dj_glove);
    ~Protocol();
    
    void ApplyNext(const unsigned char data);

private:
    static const int max_data_index = 11;
    DjGlove& m_dj_glove;
    int      m_index;

    unsigned char Prepare(const unsigned char data) const;

    bool IsHit(const unsigned char data) const;

    void ApplyData(const unsigned char data);
    void ApplyHit(const unsigned char data);

    void ApplyBit(bool& output,
                  const unsigned char data,
                  const int bit);
    void ApplyLowBits(int& output,
                      const unsigned char data,
                      const int bits);
    void ApplyBits(int& output,
                   const int offset,
                   const unsigned char data,
                   const int bits);

    void ForwardDataIndex();
};

#endif // DJ_GLOVE_PC_PROTOCOL_H_

