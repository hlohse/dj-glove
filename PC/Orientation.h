#ifndef DJ_GLOVE_PC_DJ_GLOVE_ORIENTATION_H_
#define DJ_GLOVE_PC_DJ_GLOVE_ORIENTATION_H_

#include "Export.h"
#include <utility>

#ifdef __linux__
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>
#elif _WIN32
#include <windows.h>
#endif

class EXPORT Orientation {
public:
#ifdef __linux__
    using Time = struct timeval;
#elif _WIN32
    using Time = SYSTEMTIME;
#endif
    
    static const double offset_x;
    static const double offset_y;
    
    Orientation(const double offset);

    double Value() const;

    void SetVelocityLow(const char abs);
    void SetVelocityHigh(const char signed_abs);

private:
    double m_value;
    double m_offset;
    std::pair<double, int> m_previous_velocity;
    std::pair<double, int> m_new_velocity;
};

#endif // DJ_GLOVE_PC_DJ_GLOVE_ORIENTATION_H_

