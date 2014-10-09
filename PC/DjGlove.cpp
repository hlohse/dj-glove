#include "DjGlove.h"
#include "MidiSignal.h"
#include "ControllerSwitch.h"
#include "ControllerRange.h"
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
    static const ControllerSwitch::Mode change = ControllerSwitch::Mode::OnButtonChange;
    static const ControllerSwitch::Mode press  = ControllerSwitch::Mode::OnButtonPress;
	//Instances:
	static ControllerSwitch DrumRecActivationSwitch(m_button_push_4, 0x20, change);
	static ControllerSwitch DrumLoopStartStopSwitch(m_button_touch_2, 0x21, change);
	static ControllerSwitch ThNoteStartStopSwitch(m_button_push_2, 0x22, press);
	static ControllerSwitch ThRecActivationSwitch(m_button_touch_3, 0x23, change);
	static ControllerSwitch ThLoopStartStopSwitch(m_button_push_4, 0x24, change);
	static ControllerRange  ThFlexController(m_flex, 0x25);

	//static int gyro_x = 0;
	//static ControllerRange  GyroRegler(gyro_x, 0x26);
	//gyro_x = m_orientation_x.Degree();

	switch (m_program) {
	

	case 1: //DRUMS
		
		//Hit:
		if (m_hit_intensity > 0){
			int hit_note = 36;
			MidiSignal hitSignal;
			if (m_distance > 70) hit_note += 4;
			if (m_orientation_y.Degree() > 0) hit_note += 2;
			if (m_button_touch_3) hit_note += 1;
			hitSignal.Status(Midi::Status::NoteOn);
			hitSignal.Channel(m_channel);
			hitSignal.Key(hit_note);
			hitSignal.Velocity(m_hit_intensity);
			m_midi_signals.push_back(hitSignal);

			MidiSignal off;
			off.Status(Midi::Status::NoteOff);
			off.Channel(m_channel);
			off.Key(hit_note);
			off.Velocity(m_hit_intensity);
			m_midi_signals.push_back(off);
		}
		//Gyro-Recalibration:
		if (m_button_touch_1) m_orientation_y.calibrate();

		//Record-Activation:
		if (DrumRecActivationSwitch.Changed()){
			m_midi_signals.push_back(DrumRecActivationSwitch.Signal(m_channel));
		}

		//Loop Start and Stop:
		if(DrumLoopStartStopSwitch.Changed()){
			m_midi_signals.push_back(DrumLoopStartStopSwitch.Signal(m_channel));
		}
		break;
	

	case 2: //THEREMIN:
		static int ThDistanceOldVal = 0;
		static bool ThFlipOldVal = 0;
		//StartNote:
		if (ThNoteStartStopSwitch.Changed()){
			m_midi_signals.push_back(ThNoteStartStopSwitch.Signal(m_channel));
		}
		//Record-Activation:
		if (ThRecActivationSwitch.Changed()){
			m_midi_signals.push_back(ThRecActivationSwitch.Signal(m_channel));
		}

		//Loop Start and Stop:
		if (ThLoopStartStopSwitch.Changed()){
			m_midi_signals.push_back(ThLoopStartStopSwitch.Signal(m_channel));
		}

		if (ThFlexController.Changed()) {
			m_midi_signals.push_back(ThFlexController.Signal(m_channel));
		}

		int adjusted_distance = m_distance * 129;
		if (adjusted_distance >= 0 && adjusted_distance <= 3277) {
			adjusted_distance = 4682;
		}
		else if (adjusted_distance >= 3278 && adjusted_distance <= 6553) {
			adjusted_distance = 7022;
		}
		else if (adjusted_distance >= 6554 && adjusted_distance <= 9830) {
			adjusted_distance = 8192;
		}
		else if (adjusted_distance >= 9831 && adjusted_distance <= 13106) {
			adjusted_distance = 12871;
		}
		else {
			adjusted_distance = 16383;
		}

		if (adjusted_distance != ThDistanceOldVal) {
			MidiSignal pitchsignal;
			pitchsignal.Status(Midi::Status::PitchBend);
			pitchsignal.Channel(m_channel);
			pitchsignal.PitchBend(adjusted_distance);
			m_midi_signals.push_back(pitchsignal);
			ThDistanceOldVal = adjusted_distance;
		}

		if (m_button_flip != ThFlipOldVal){
			MidiSignal flipsignal;
			flipsignal.Status(Midi::Status::ControllerChange);
			flipsignal.Channel(m_channel);
			flipsignal.Controller((Midi::Controller) 0x26);
			flipsignal.ControllerValue(m_button_flip * 127);
			ThFlipOldVal = m_button_flip;
			m_midi_signals.push_back(flipsignal);
		}

		//if (m_button_push_4 && GyroRegler.Changed()) {
		//	m_midi_signals.push_back(GyroRegler.Signal(m_channel));
		//}
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

