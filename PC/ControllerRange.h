#ifndef DJ_GLOVE_PC_CONTROLLER_RANGE_H_
#define DJ_GLOVE_PC_CONTROLLER_RANGE_H_

#include "Export.h"
#include "Midi.h"
#include "MidiSignal.h"

class EXPORT ControllerRange {
public:
	ControllerRange(int& value, const int controller_number);

	bool Changed();
	MidiSignal Signal(const Midi::byte_t channel);

private:
	int* m_value;
	int  m_current_value;
	Midi::Controller m_controller;
};

#endif // DJ_GLOVE_PC_CONTROLLER_RANGE_H_
