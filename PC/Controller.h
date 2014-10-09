#ifndef DJ_GLOVE_PC_CONTROLLER_H_
#define DJ_GLOVE_PC_CONTROLLER_H_

#include "Export.h"
#include "Midi.h"
#include "MidiSignal.h"

class EXPORT Controller {
public:
    Controller(const int controller_number)
    :   m_controller(controller_number)
    {
    }

    virtual bool Changed() = 0;
    virtual MidiSignal Signal(const int channel) = 0;

protected:
    int m_controller;
};

#endif // DJ_GLOVE_PC_CONTROLLER_H_

