#include "ControllerRange.h"

ControllerRange::ControllerRange(int& value, const int controller_number)
:   Controller(controller_number),
    m_value(&value),
    m_current_value(-1)
{
}

bool ControllerRange::Changed()
{
	if (*m_value != m_current_value) {
		m_current_value = *m_value;
		return true;
	}
	return false;
}

MidiSignal ControllerRange::Signal(const int channel)
{
    return {Midi::Status::ControllerChange, channel, m_controller, m_current_value};
}
