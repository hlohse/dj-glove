#ifndef DJ_GLOVE_PC_CONTROLLER_RANGE_H_
#define DJ_GLOVE_PC_CONTROLLER_RANGE_H_

#include "Controller.h"

class EXPORT ControllerRange : public Controller {
public:
	ControllerRange(int& value, const int controller_number);

    // Inherited from Controller
	virtual bool Changed();
	virtual MidiSignal Signal(const int channel);

private:
	int* m_value;
	int  m_current_value;
};

#endif // DJ_GLOVE_PC_CONTROLLER_RANGE_H_
