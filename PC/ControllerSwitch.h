#ifndef DJ_GLOVE_PC_CONTROLLER_SWITCH_H_
#define DJ_GLOVE_PC_CONTROLLER_SWITCH_H_

#include "Controller.h"

class EXPORT ControllerSwitch : public Controller {
public:
    enum class Mode : char {
        OnButtonChange, // Pressed: high, released: low
        OnButtonPress   // Pressed changes high to low or low to high
    };

    ControllerSwitch(
        bool&      button,
        const int  controller_number,
        const Mode mode,
        const int  low_value = 0,
        const int  high_value = 127);

    // Inherited from Controller
    virtual bool Changed();
    virtual MidiSignal Signal(const int channel);

protected:
    bool* m_button;
    bool  m_old_state;
    bool  m_is_activated;
    bool* m_high_value_reference;
    Midi::byte_t m_low_value;
    Midi::byte_t m_high_value;
    bool (ControllerSwitch::*m_changed_strategy)();

    bool ButtonChanged();
    bool ButtonPressed();
};

#endif // DJ_GLOVE_PC_CONTROLLER_SWITCH_H_
