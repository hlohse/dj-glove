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

#ifndef DJ_GLOVE_PC_DATA_PROTOCOL_H_
#define DJ_GLOVE_PC_DATA_PROTOCOL_H_

#include "Export.h"

class DjGlove;

class EXPORT DataProtocol {
public:
    DataProtocol(DjGlove& dj_glove);
    ~DataProtocol();
    
    void ApplyNext(const unsigned char data);

private:
    static const int max_data_index = 10;
    DjGlove& dj_glove_;
    int      data_index_;

    unsigned char Prepare(const unsigned char data) const;

    bool IsPunch(const unsigned char data) const;

    void ApplyData(const unsigned char data);
    void ApplyPunch(const unsigned char data);

    void ApplyBit(bool& output, const unsigned char data, const int bit);
    void ApplyLowBits(int& output, const unsigned char data, const int bits);
    
    void ForwardDataIndex();
};

#endif // DJ_GLOVE_PC_DATA_PROTOCOL_H_

