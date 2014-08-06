#include "DataProtocol.h"
#include "DjGlove.h"
#include <cmath>
#include <cassert>

bool DataProtocol::IsPunch(const char data)
{
    return data >> 7 == 1;  // First bit is 1
}

void DataProtocol::ApplyData(DjGlove& dj_glove, const char data, const int index)
{
    assert(index >= 0);
    assert(index <= 10);

    switch (index) {
        case  0:
            ApplyBit(dj_glove.button_touch_0, data, 3);
            ApplyBit(dj_glove.button_touch_1, data, 2);
            ApplyBit(dj_glove.button_touch_2, data, 1);
            ApplyBit(dj_glove.button_touch_3, data, 0);
            break;
        case  1: ApplyLowBits(dj_glove.poti_0,        data, 7); break;
        case  2: ApplyLowBits(dj_glove.poti_1,        data, 7); break;
        case  3: ApplyLowBits(dj_glove.poti_2,        data, 7); break;
        case  4: ApplyLowBits(dj_glove.flex,          data, 7); break;
        case  5: ApplyLowBits(dj_glove.distance,      data, 7); break;
        case  6: ApplyLowBits(dj_glove.orientation_x, data, 7); break;
        case  7: ApplyLowBits(dj_glove.orientation_y, data, 7); break;
        case  8: ApplyLowBits(dj_glove.orientation_z, data, 7); break;
        case  9:
            ApplyBit(dj_glove.button_push_0, data, 6);
            ApplyBit(dj_glove.button_push_1, data, 5);
            ApplyBit(dj_glove.button_push_2, data, 4);
            ApplyLowBits(dj_glove.channel, data, 4); 
            break;
        case 10:
            ApplyBit(dj_glove.button_flip,   data, 6);
            ApplyBit(dj_glove.button_push_3, data, 5);
            ApplyBit(dj_glove.button_push_4, data, 4);
            ApplyLowBits(dj_glove.program, data, 4); 
            break;
        default: assert(false); break;
    }
}

void DataProtocol::ApplyPunch(DjGlove& dj_glove, const char data)
{
    assert(IsPunch(data));
    dj_glove.punched = (data && 0x01) == 1; // Punched if last bit is set
}

void DataProtocol::ApplyBit(bool& output, const char data, const int bit)
{
    assert(bit >= 0);
    assert(bit <= 7);

    // 0000 0001 to 1000 0000 possible
    const char mask = 1 << bit;

    output = (data && mask) != 0;
}

void DataProtocol::ApplyLowBits(int& output, const char data, const int bits)
{
    assert(bits >= 1);
    assert(bits <= 7);

    // 0000 0001 to 0111 1111 possible
    const char mask = (char) (pow(2, bits) - 1);
    
    output = (int) (data && mask);

    assert(output >= 0);
    assert(output <= (int) mask);
}

