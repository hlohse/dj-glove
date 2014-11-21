#ifndef DJ_GLOVE_PC_CONTROLLER_GYRO_H_
#define DJ_GLOVE_PC_CONTROLLER_GYRO_H_

#include "Export.h"
#include "ControllerSwitch.h"
#include "Orientation.h"

class EXPORT ControllerGyro : public ControllerSwitch {
public:
    ControllerGyro(bool& reference_value,
                   Orientation& orientation,
                   const int controller_number);

    // Inerited from Controller
    virtual bool Changed();
    virtual MidiSignal Signal(const int channel);

private:
    Orientation* m_orientation;
    int m_initial_degree;
    int m_previous_degree;
    int m_previous_controller_value;
    int m_new_controller_value;
};

#endif // DJ_GLOVE_PC_CONTROLLER_GYRO_H_

