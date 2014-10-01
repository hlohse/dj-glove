#include "DjGlove.h"
#include "MidiSignal.h"
#include <sstream>
#include <cassert>
using namespace std;

DjGlove::DjGlove()
:   data_protocol_(Protocol(*this)),
    m_hit_intensity(0),
    m_button_push_0(false),
    m_button_push_1(false),
    m_button_push_2(false),
    m_button_push_3(false),
    m_button_push_4(false),
    m_button_touch_0(false),
    m_button_touch_1(false),
    m_button_touch_2(false),
    m_button_touch_3(false),
    m_button_flip(false),
    m_poti_0(0),
    m_poti_1(0),
    m_poti_2(0),
    m_distance(0),
    m_orientation_x(0),
    m_orientation_y(0),
    m_orientation_z(0),
    m_flex(0),
    m_channel(0),
    program_(0)
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

    midi_signal.Channel(m_channel);
    midi_signal.Status(Midi::Status::NoteOn);
    midi_signal.Key(m_poti_0);
    midi_signal.Velocity(m_poti_1);

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

	data << m_hit_intensity << " "
         << BoolToText(m_button_push_0)
            << BoolToText(m_button_push_1)
            << BoolToText(m_button_push_2)
            << BoolToText(m_button_push_3)
            << BoolToText(m_button_push_4) << " "
        << BoolToText(m_button_touch_0)
            << BoolToText(m_button_touch_1)
            << BoolToText(m_button_touch_2)
            << BoolToText(m_button_touch_3) << " "
        << BoolToText(m_button_flip) << " "
        << m_poti_0 << " "
        << m_poti_1 << " "
        << m_poti_2 << " "
        << m_distance << " "
        << m_orientation_x << " "
        << m_orientation_y << " "
        << m_orientation_z << " "
        << m_flex << " "
        << m_channel << " "
        << program_;

    return data.str();
}

char DjGlove::BoolToText(const bool value) const
{
    return value ? 'Y' : 'N';
}

