#ifndef DJ_GLOVE_PC_CONTROLLER_PUSH_H_
#define DJ_GLOVE_PC_CONTROLLER_PUSH_H_

#include "Export.h"
#include "Midi.h"
#include "MidiSignal.h"

class EXPORT ControllerPush {
public:
	ControllerPush(
		bool& button,
		const int controller_number,
		const Midi::byte_t low_value = 0,
		const Midi::byte_t high_value = 127);

	bool Actuated();
	MidiSignal Signal(const Midi::byte_t channel);

private:
	bool* m_button;
	bool  m_old_state;
	Midi::Controller m_controller;
	Midi::byte_t m_low_value;
	Midi::byte_t m_high_value;
};

#endif // DJ_GLOVE_PC_CONTROLLER_SWITCH_H_
