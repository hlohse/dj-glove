#include "Protocol.h"
#include "DjGlove.h"
#include <cmath>
#include <cassert>

Protocol::Protocol(DjGlove& dj_glove)
:   m_dj_glove(dj_glove),
    m_index(0)
{
}

Protocol::~Protocol()
{
}

void Protocol::ApplyNext(const unsigned char data)
{
    const unsigned char prepared_data = Prepare(data);

    if (IsHit(prepared_data)) {
        ApplyHit(prepared_data);
    }
    else {
        ApplyData(prepared_data);
        ForwardDataIndex();
    }
}

unsigned char Protocol::Prepare(const unsigned char data) const
{
/*  All values have to be decremented by 1. This is because zero/null bytes can
 *  not be send via Bluetooth and are therefore increment by 1.
 */
    return data - 1;
}

bool Protocol::IsHit(const unsigned char data) const
{
    return data >> 7 == 1;  // First bit is 1
}

void Protocol::ApplyHit(const unsigned char data)
{
    assert(IsHit(data));
	m_dj_glove.m_hit_intensity = data & 0x7E; // 0111 1110
}

void Protocol::ApplyData(const unsigned char data)
{
    assert(m_index >= 0);
    assert(m_index <= max_data_index);

    switch (m_index) {
        case  0:
            ApplyBit(m_dj_glove.m_button_touch_2, data, 3);
            ApplyBit(m_dj_glove.m_button_touch_1, data, 2);
            ApplyBit(m_dj_glove.m_button_touch_0, data, 1);
            ApplyBit(m_dj_glove.m_button_touch_3, data, 0);
            break;
        case  1: ApplyLowBits(m_dj_glove.m_poti_2,        data, 7); break;
        case  2: ApplyLowBits(m_dj_glove.m_poti_1,        data, 7); break;
        case  3: ApplyLowBits(m_dj_glove.m_poti_0,        data, 7); break;
        case  4: ApplyLowBits(m_dj_glove.m_flex,          data, 7); break;
        case  5: ApplyLowBits(m_dj_glove.m_distance,      data, 7); break;
        case  6: ApplyBits(m_dj_glove.m_orientation_x, 0, data, 7); break;
        case  7:
            ApplyBits(m_dj_glove.m_orientation_x, 7, data, 6);
            m_dj_glove.m_orientation_x |= (data & 0x40) != 0 ?
                (int) pow(2, sizeof(m_dj_glove.m_orientation_x) * 8 - 1) :
                0;
            break;
        case  8: ApplyBits(m_dj_glove.m_orientation_y, 0, data, 7); break;
        case  9:
            ApplyBits(m_dj_glove.m_orientation_y, 7, data, 6);
            m_dj_glove.m_orientation_y |= (data & 0x40) != 0 ?
                (int) pow(2, sizeof(m_dj_glove.m_orientation_y) * 8) :
                0;
            break;
        case 10:
            ApplyBit(m_dj_glove.m_button_push_4, data, 6);
            ApplyBit(m_dj_glove.m_button_push_3, data, 5);
            ApplyBit(m_dj_glove.m_button_push_2, data, 4);
            ApplyLowBits(m_dj_glove.m_channel,   data, 4); 
            break;
        case 11:
            ApplyBit(m_dj_glove.m_button_flip,   data, 6);
            ApplyBit(m_dj_glove.m_button_push_1, data, 5);
            ApplyBit(m_dj_glove.m_button_push_0, data, 4);
            ApplyLowBits(m_dj_glove.m_program,   data, 4); 
            break;
        default: assert(false); break;
    }
}

void Protocol::ApplyBit(bool& output,
                        const unsigned char data,
                        const int bit)
{
    assert(bit >= 0);
    assert(bit <= 7);

    // 0000 0001 to 1000 0000 possible
    const unsigned char mask = 1 << bit;

    output = (data & mask) != 0;
}

void Protocol::ApplyLowBits(int& output,
                            const unsigned char data,
                            const int bits)
{
    assert(bits >= 1);
    assert(bits <= 7);

    // 0000 0001 to 0111 1111 possible
    const unsigned char mask = (unsigned char) (pow(2, bits) - 1);
    
    output = (int) (data & mask);

    assert(output >= 0);
    assert(output <= (int) mask);
}

void Protocol::ApplyBits(int& output,
                         const int offset,
                         const unsigned char data,
                         const int bits)
{
    assert(offset >= 0);
    assert(bits   >= 1);
    assert(bits   <= 7);

    // 0000 0001 to 0111 1111 possible
    const unsigned char mask      = (unsigned char) (pow(2, bits) - 1);
    const unsigned char data_bits = data & mask;

    const int output_bits = ((int) data_bits) << offset;
    output |= output_bits;

    assert(output >= 0);
}

void Protocol::ForwardDataIndex()
{
    m_index++;

    if (m_index > max_data_index) {
        m_index = 0;
    }
}

