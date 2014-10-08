#include "ControllerSwitch.h"

ControllerSwitch::ControllerSwitch(
	bool& button,
	const int controller_number,
	const Midi::byte_t low_value,
	const Midi::byte_t high_value)
:	m_button(&button),
	m_old_state(false),
	m_is_activated(false),
	m_controller((Midi::Controller) controller_number),
	m_low_value(low_value),
	m_high_value(high_value)
{
}

bool ControllerSwitch::Switched()
{
	bool result = false;

	if (*m_button && !m_old_state) {
		m_is_activated = !m_is_activated;
		result = true;
	}

	m_old_state = *m_button;

	return result;
}

MidiSignal ControllerSwitch::Signal(const Midi::byte_t channel)
{
	MidiSignal controller_signal;

	controller_signal.Status(Midi::Status::ControllerChange);
	controller_signal.Channel(channel);
	controller_signal.Controller(m_controller);

	if (m_is_activated) {
		controller_signal.ControllerValue(m_high_value);
	}
	else {
		controller_signal.ControllerValue(m_low_value);
	}
	return controller_signal;
}
