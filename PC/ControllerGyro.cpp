#include "ControllerGyro.h"

ControllerGyro::ControllerGyro(bool& reference_value,
                               Orientation& orientation,
                               const int controller_number)
:   ControllerSwitch(reference_value,
                     controller_number,
                     ControllerSwitch::Mode::OnButtonChange),
    m_orientation(&orientation),
    m_initial_degree(0),
    m_previous_controller_value(127),
    m_new_controller_value(127)
{
}

bool ControllerGyro::Changed()
{
    if (ControllerSwitch::Changed()) {
        if (*m_button) {
            m_initial_degree = m_orientation->Degree();
            return true;
        }
        else {
            m_previous_controller_value = m_new_controller_value;
            return false;
        }
    }
    else if (*m_button) {
        return m_initial_degree != m_orientation->Degree();
    }

    return false;
}

MidiSignal ControllerGyro::Signal(const int channel)
{
    m_new_controller_value = m_previous_controller_value
        - (m_orientation->Degree() - m_initial_degree);

    return {Midi::Status::ControllerChange, channel, m_controller,
            m_new_controller_value};
}

