#include "Orientation.h"
using namespace std;

// 10:24.5 @ 100k
const double Orientation::offset_x = 4.27213;
const double Orientation::offset_y = -0.666;
const double Orientation::value_per_90_degrees = 330000;

Orientation::Orientation(const double offset)
:   m_value(0),
    m_offset(offset),
    m_previous_velocity(pair<double, int>(0, 0)),
    m_new_velocity(pair<double, int>(0, 0))
{
}

int Orientation::Degree() const
{
    return (int) (90.0 * (m_value / value_per_90_degrees));
}
    
void Orientation::SetVelocityLow(const char abs)
{
    m_previous_velocity = m_new_velocity;

    // 0111 1111, lowest 7 bits
    m_new_velocity.second = abs & 0x7F;
}

void Orientation::SetVelocityHigh(const char signed_abs)
{
    Time timestamp;

    // 0011 1111, lowest 6 bits placed after first 7
    m_new_velocity.second |= ((int) (signed_abs & 0x3F)) << 7;

    // If X1XX XXXX, value is negative
    m_new_velocity.second *= signed_abs & 0x40 ? -1 : 1;

#ifdef __linux__
    gettimeofday(&timestamp, NULL);
    m_new_velocity.first = timestamp.tv_sec * 1e3
                         + ((double) timestamp.tv_usec) / 1e3;
#elif _WIN32
    GetSystemTime(&timestamp);
    m_new_velocity.first = (double) timestamp.wMilliseconds;
#endif

    if (m_previous_velocity.first != 0) {
        const double dtime  = m_new_velocity.first - m_previous_velocity.first;
        m_value += dtime * (m_new_velocity.second - m_offset);
    }
}

