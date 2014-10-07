#include "DjGlove.h"
#include "MidiSignal.h"
#include <sstream>
#include <cassert>
using namespace std;

DjGlove::DjGlove()
:   m_data_protocol(Protocol(*this)),
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
    m_orientation_x(Orientation::offset_x),
    m_orientation_y(Orientation::offset_y),
    m_flex(0),
    m_channel(0),
    m_program(0)
{
}

DjGlove::~DjGlove()
{
}

void DjGlove::Process(const char data)
{
    m_data_protocol.ApplyNext(data);
    GenerateMidiSignals();
}

void DjGlove::GenerateMidiSignals()
{
	switch (m_program) {
	case 1:
		if (m_hit_intensity > 0){
			MidiSignal hitSignal;
			hitSignal.Status(Midi::Status::NoteOn);
			hitSignal.Channel(m_channel);
			hitSignal.Key(60);
			hitSignal.Velocity(m_hit_intensity);
			m_midi_signals.push_back(hitSignal);
		}

		break;
	}
}

bool DjGlove::HasMidiSignal() const
{
    return !m_midi_signals.empty();
}

MidiSignal DjGlove::NextMidiSignal()
{
    assert(HasMidiSignal());

    MidiSignal next_midi_signal = m_midi_signals.front();
    m_midi_signals.pop_front();
    
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
    header << "XY  Orientations (0..16383)" << endl;
    header << "F   Flex (0..127)" << endl;
    header << "C   Channel (0..9)" << endl;
    header << "P   Program (0..9)" << endl;
    
    header << "HHH ppppp tttt f O   O   O    D   X   Y   F   C P" << endl;

    return header.str();
}

string DjGlove::DataString() const
{
    ostringstream data;

	data << m_hit_intensity << "\t"
         << BoolToText(m_button_push_0)
            << BoolToText(m_button_push_1)
            << BoolToText(m_button_push_2)
            << BoolToText(m_button_push_3)
            << BoolToText(m_button_push_4) << "\t"
        << BoolToText(m_button_touch_0)
            << BoolToText(m_button_touch_1)
            << BoolToText(m_button_touch_2)
            << BoolToText(m_button_touch_3) << "\t"
        << BoolToText(m_button_flip) << "\t"
        << m_poti_0 << "\t"
        << m_poti_1 << "\t"
        << m_poti_2 << "\t"
        << m_distance << "\t"
        << m_orientation_x.Degree() << "\t"
        << m_orientation_y.Degree() << "\t"
        << m_flex << "\t"
        << m_channel << "\t"
        << m_program;

    return data.str();
}

char DjGlove::BoolToText(const bool value) const
{
    return value ? 'Y' : 'N';
}

