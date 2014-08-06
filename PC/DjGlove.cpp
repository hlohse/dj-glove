#include "DjGlove.h"
#include "DataProtocol.h"
#include "MidiSignal.h"
#include <cassert>

DjGlove::DjGlove()
:   punched_(false),
    button_push_0_(false),
    button_push_1_(false),
    button_push_2_(false),
    button_push_3_(false),
    button_push_4_(false),
    button_touch_0_(false),
    button_touch_1_(false),
    button_touch_2_(false),
    button_touch_3_(false),
    button_flip_(false),
    poti_0_(0),
    poti_1_(0),
    poti_2_(0),
    distance_(0),
    orientation_x_(0),
    orientation_y_(0),
    orientation_z_(0),
    flex_(0),
    channel_(0),
    program_(0),
    data_byte_index_(0)
{
}

DjGlove::~DjGlove()
{
}

void DjGlove::Process(const char data)
{
    ApplyData(data);
    GenerateMidiSignals();
}

void DjGlove::ApplyData(const char data)
{
    if (DataProtocol::IsPunch(data)) {
        DataProtocol::ApplyPunch(*this, data);
    }
    else {
        DataProtocol::ApplyData(*this, data, data_byte_index_);
        data_byte_index_++;

        if (data_byte_index_ > DataProtocol::max_index) {
            data_byte_index_ = 0;
        }
    }
}

void DjGlove::GenerateMidiSignals()
{
    MidiSignal midi_signal;

    midi_signal.Channel(channel_);
    midi_signal.Status(button_push_0_ ? Midi::Status::NoteOn
                                      : Midi::Status::NoteOff);
    midi_signal.Key(poti_0_);
    midi_signal.Velocity(63);
}

bool DjGlove::HasMidiSignal() const
{
    return !midi_signals_.empty();
}

MidiSignal DjGlove::NextMidiSignal()
{
    assert(HasMidiSignal());

    MidiSignal next_midi_signal = midi_signals_.front();
    midi_signals_.pop_front();
    
    return next_midi_signal;
}

