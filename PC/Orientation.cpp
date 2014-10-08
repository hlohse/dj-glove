#include "Orientation.h"
#ifdef __linux__
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>
#elif _WIN32
#include <windows.h>
#endif
using namespace std;

// 10:24.5 @ 100k
const double Orientation::offset_x = 4.27213;
const double Orientation::offset_y = -0.666;
const double Orientation::value_per_90_degrees = 330000;

long long Orientation::frequency;

void Orientation::InitFrequency()
{
	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);
	Orientation::frequency = frequency.QuadPart;
}

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
    // 0011 1111, lowest 6 bits placed after first 7
    m_new_velocity.second |= ((int) (signed_abs & 0x3F)) << 7;

    // If X1XX XXXX, value is negative
    m_new_velocity.second *= signed_abs & 0x40 ? -1 : 1;

#ifdef __linux__
	struct timeval timestamp;
    gettimeofday(&timestamp, NULL);
    m_new_velocity.first = timestamp.tv_sec * 1e3
                         + ((double) timestamp.tv_usec) / 1e3;
#elif _WIN32
	LARGE_INTEGER count;
	QueryPerformanceCounter(&count);
	m_new_velocity.first = ((double)count.QuadPart) / ((double)Orientation::frequency) * 1e3;
#endif

    if (m_previous_velocity.first != 0) {
        const double dtime  = m_new_velocity.first - m_previous_velocity.first;
        m_value += dtime * (m_new_velocity.second - m_offset);
    }
}

void Orientation::calibrate(){
	m_value = 0;
}

