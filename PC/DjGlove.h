#ifndef DJ_GLOVE_PC_DJ_GLOVE_H_
#define DJ_GLOVE_PC_DJ_GLOVE_H_

#include "Export.h"
#include "Protocol.h"
#include "MidiSignal.h"
#include "Orientation.h"
#include <deque>
#include <string>

class EXPORT DjGlove {
    friend class Protocol;

public:
    DjGlove();
    ~DjGlove();

    void Process(const char data);

    bool HasMidiSignal() const;
    MidiSignal NextMidiSignal();

    std::string DataHeader() const;
    std::string DataString() const;

private:
    Protocol m_data_protocol;
    std::deque<MidiSignal> m_midi_signals;

    int  m_hit_intensity;
    bool m_button_push_0;
    bool m_button_push_1;
    bool m_button_push_2;
    bool m_button_push_3;
    bool m_button_push_4;
    bool m_button_touch_0;
    bool m_button_touch_1;
    bool m_button_touch_2;
    bool m_button_touch_3;
    bool m_button_flip;
    int  m_poti_0;
    int  m_poti_1;
    int  m_poti_2;
    int  m_distance;
    Orientation m_orientation_x;
    Orientation m_orientation_y;
    int  m_flex;
    int  m_channel;
    int  m_program;

    void GenerateMidiSignals();
    void Register(const MidiSignal& midi_signal);

    char BoolToText(const bool value) const;
};

#endif // DJ_GLOVE_PC_DJ_GLOVE_H_

