#include "ControllerSwitch.h"

ControllerSwitch::ControllerSwitch(
    bool&      button,
    const int  controller_number,
    const Mode mode,
    const int  low_value,
    const int  high_value)
:    Controller(controller_number),
    m_button(&button),
    m_old_state(false),
    m_is_activated(false),
    m_low_value((Midi::byte_t) low_value),
    m_high_value((Midi::byte_t) high_value)
{
    switch (mode) {
        case Mode::OnButtonChange:
            m_high_value_reference = m_button;
            m_changed_strategy = &ControllerSwitch::ButtonChanged;
            break;
        default: // OnButtonPress
            m_high_value_reference = &m_is_activated;
            m_changed_strategy = &ControllerSwitch::ButtonPressed;
            break;
    }
}

bool ControllerSwitch::Changed()
{
    return (this->*m_changed_strategy)();
}

bool ControllerSwitch::ButtonChanged()
{
    bool result = false;

    if (*m_button != m_old_state) {
        result = true;
    }

    m_old_state = *m_button;
    return result;
}

bool ControllerSwitch::ButtonPressed()
{
    bool result = false;

    if (*m_button && !m_old_state) {
        m_is_activated = !m_is_activated;
        result = true;
    }

    m_old_state = *m_button;
    return result;
}

MidiSignal ControllerSwitch::Signal(const int channel)
{
    const int value = *m_high_value_reference ? m_high_value : m_low_value;
    return {Midi::Status::ControllerChange, channel, m_controller, value};
}
