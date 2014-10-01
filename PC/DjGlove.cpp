#include "DjGlove.h"
#include "MidiSignal.h"
#include <sstream>
#include <cassert>
using namespace std;

DjGlove::DjGlove()
:   data_protocol_(Protocol(*this)),
    hit_intensity_(0),
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

void DjGlove::Process(const unsigned char data)
{
    data_protocol_.ApplyNext(data);
    GenerateMidiSignals();
}

void DjGlove::GenerateMidiSignals()
{
    MidiSignal midi_signal;

    midi_signal.Channel(channel_);
    midi_signal.Status(Midi::Status::NoteOn);
    midi_signal.Key(poti_0_);
    midi_signal.Velocity(poti_1_);

    midi_signals_.push_back(midi_signal);
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

string DjGlove::DataHeader() const
{
    ostringstream header;
    
    header << "HHH Hit intensity (0..126)" << endl;
    header << "p   Push button pressed (Y/N)" << endl;
    header << "t   Touch button touched (Y/N)" << endl;
    header << "f   Flip button flipped (Y/N)" << endl;
    header << "O   Poti state (0..127)" << endl;
    header << "D   Distance (0..127)" << endl;
    header << "XYZ Orientations (0..127)" << endl;
    header << "F   Flex (0..127)" << endl;
    header << "C   Channel (0..9)" << endl;
    header << "P   Program (0..9)" << endl;
    
    header << "HHH ppppp tttt f O   O   O    D   X   Y   Z   F   C P" << endl;

    return header.str();
}

string DjGlove::DataString() const
{
    ostringstream data;

	data << hit_intensity_ << " "
         << BoolToText(button_push_0_)
            << BoolToText(button_push_1_)
            << BoolToText(button_push_2_)
            << BoolToText(button_push_3_)
            << BoolToText(button_push_4_) << " "
        << BoolToText(button_touch_0_)
            << BoolToText(button_touch_1_)
            << BoolToText(button_touch_2_)
            << BoolToText(button_touch_3_) << " "
        << BoolToText(button_flip_) << " "
        << poti_0_ << " "
        << poti_1_ << " "
        << poti_2_ << " "
        << distance_ << " "
        << orientation_x_ << " "
        << orientation_y_ << " "
        << orientation_z_ << " "
        << flex_ << " "
        << channel_ << " "
        << program_;

    return data.str();
}

char DjGlove::BoolToText(const bool value) const
{
    return value ? 'Y' : 'N';
}

