#include "DataProtocol.h"
#include "DjGlove.h"
#include <cmath>
#include <cassert>
    
DataProtocol::DataProtocol(DjGlove& dj_glove)
:   dj_glove_(dj_glove),
    data_index_(0)
{
}

DataProtocol::~DataProtocol()
{
}

void DataProtocol::ApplyNext(const unsigned char data)
{
    const unsigned char prepared_data = Prepare(data);

    if (IsPunch(prepared_data)) {
        ApplyPunch(prepared_data);
    }
    else {
        ApplyData(prepared_data);
        ForwardDataIndex();
    }
}

unsigned char DataProtocol::Prepare(const unsigned char data) const
{
/*  All values have to be decremented by 1. This is because zero/null bytes can
 *  not be send via Bluetooth and are therefore increment by 1.
 */
    return data - 1;
}

bool DataProtocol::IsPunch(const unsigned char data) const
{
    return data >> 7 == 1;  // First bit is 1
}

void DataProtocol::ApplyPunch(const unsigned char data)
{
    assert(IsPunch(data));
    dj_glove_.punched_ = (data && 0x01) == 1; // Punched if last bit is set
}

void DataProtocol::ApplyData(const unsigned char data)
{
    assert(data_index_ >= 0);
    assert(data_index_ <= max_data_index);

    switch (data_index_) {
        case  0:
            ApplyBit(dj_glove_.button_touch_2_, data, 3);
            ApplyBit(dj_glove_.button_touch_1_, data, 2);
            ApplyBit(dj_glove_.button_touch_0_, data, 1);
            ApplyBit(dj_glove_.button_touch_3_, data, 0);
            break;
        case  1: ApplyLowBits(dj_glove_.poti_2_,        data, 7); break;
        case  2: ApplyLowBits(dj_glove_.poti_1_,        data, 7); break;
        case  3: ApplyLowBits(dj_glove_.poti_0_,        data, 7); break;
        case  4: ApplyLowBits(dj_glove_.flex_,          data, 7); break;
        case  5: ApplyLowBits(dj_glove_.distance_,      data, 7); break;
        case  6: ApplyLowBits(dj_glove_.orientation_x_, data, 7); break;
        case  7: ApplyLowBits(dj_glove_.orientation_y_, data, 7); break;
        case  8: ApplyLowBits(dj_glove_.orientation_z_, data, 7); break;
        case  9:
            ApplyBit(dj_glove_.button_push_4_, data, 6);
            ApplyBit(dj_glove_.button_push_3_, data, 5);
            ApplyBit(dj_glove_.button_push_2_, data, 4);
            ApplyLowBits(dj_glove_.channel_,   data, 4); 
            break;
        case 10:
            ApplyBit(dj_glove_.button_flip_,   data, 6);
            ApplyBit(dj_glove_.button_push_1_, data, 5);
            ApplyBit(dj_glove_.button_push_0_, data, 4);
            ApplyLowBits(dj_glove_.program_,   data, 4); 
            break;
        default: assert(false); break;
    }
}

void DataProtocol::ApplyBit(bool& output, const unsigned char data, const int bit)
{
    assert(bit >= 0);
    assert(bit <= 7);

    // 0000 0001 to 1000 0000 possible
    const unsigned char mask = 1 << bit;

    output = (data & mask) != 0;
}

void DataProtocol::ApplyLowBits(int& output, const unsigned char data, const int bits)
{
    assert(bits >= 1);
    assert(bits <= 7);

    // 0000 0001 to 0111 1111 possible
    const unsigned char mask = (unsigned char) (pow(2, bits) - 1);
    
    output = (int) (data & mask);

    assert(output >= 0);
    assert(output <= (int) mask);
}

void DataProtocol::ForwardDataIndex()
{
    data_index_++;

    if (data_index_ > max_data_index) {
        data_index_ = 0;
    }
}

