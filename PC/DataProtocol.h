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

struct DjGlove;

class DataProtocol {
public:
    static bool IsPunch(const char data);

    static void ApplyData(DjGlove& dj_glove, const char data, const int index);
    static void ApplyPunch(DjGlove& dj_glove, const char data);

private:
    static void ApplyBit(bool& output, const char data, const int bit);
    static void ApplyLowBits(int& output, const char data, const int bits);
};

#endif // DJ_GLOVE_PC_DATA_PROTOCOL_H_

