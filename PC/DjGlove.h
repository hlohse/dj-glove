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

    int  hit_intensity_;
    bool button_push_0_;
    bool button_push_1_;
    bool button_push_2_;
    bool button_push_3_;
    bool button_push_4_;
    bool button_touch_0_;
    bool button_touch_1_;
    bool button_touch_2_;
    bool button_touch_3_;
    bool button_flip_;
    int  poti_0_;
    int  poti_1_;
    int  poti_2_;
    int  distance_;
    int  orientation_x_;
    int  orientation_y_;
    int  orientation_z_;
    int  flex_;
    int  channel_;
    int  program_;
    int  data_byte_index_;

    void GenerateMidiSignals();

    char BoolToText(const bool value) const;
};

#endif // DJ_GLOVE_PC_DJ_GLOVE_H_

