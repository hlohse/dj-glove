#ifndef DJ_GLOVE_PC_DJ_GLOVE_H_
#define DJ_GLOVE_PC_DJ_GLOVE_H_

#include "Export.h"
#include "Protocol.h"
#include "MidiSignal.h"
#include <deque>
#include <string>

class EXPORT DjGlove {
    friend class Protocol;

public:
    DjGlove();
    ~DjGlove();

    void Process(const unsigned char data);

    bool HasMidiSignal() const;
    MidiSignal NextMidiSignal();

    std::string DataHeader() const;
    std::string DataString() const;

private:
    Protocol data_protocol_;
    std::deque<MidiSignal> midi_signals_;

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
    int  m_orientation_x;
    int  m_orientation_y;
    int  m_orientation_z;
    int  m_flex;
    int  m_channel;
    int  program_;

    void GenerateMidiSignals();

    char BoolToText(const bool value) const;
};

#endif // DJ_GLOVE_PC_DJ_GLOVE_H_

