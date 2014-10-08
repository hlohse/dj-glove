#include "ControllerRange.h"

ControllerRange::ControllerRange(int& value, const int controller_number)
:   m_value(&value),
    m_current_value(-1),
	m_controller((Midi::Controller) controller_number)
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

MidiSignal ControllerRange::Signal(const Midi::byte_t channel)
{
	MidiSignal signal;
	signal.Status(Midi::Status::ControllerChange);
	signal.Channel(channel);
	signal.Controller(m_controller);
	signal.ControllerValue(m_current_value);
	return signal;
}
