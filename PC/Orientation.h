#ifndef DJ_GLOVE_PC_DJ_GLOVE_ORIENTATION_H_
#define DJ_GLOVE_PC_DJ_GLOVE_ORIENTATION_H_

#include "Export.h"
#include <utility>

class EXPORT Orientation {
public:
    static const double offset_x;
    static const double offset_y;

	static long long frequency;
	static void InitFrequency();
    
    Orientation(const double offset);

    int Degree() const;

    void SetVelocityLow(const char abs);
    void SetVelocityHigh(const char signed_abs);

	void calibrate();

private:
    static const double value_per_90_degrees;

    double m_value;
    double m_offset;
    std::pair<double, int> m_previous_velocity;
    std::pair<double, int> m_new_velocity;
};

#endif // DJ_GLOVE_PC_DJ_GLOVE_ORIENTATION_H_

