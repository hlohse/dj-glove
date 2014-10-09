#include "DjGlove.h"
#include "MidiSignal.h"
#include "ControllerSwitch.h"
#include "ControllerRange.h"
#include "ControllerPush.h"
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
	//Instances:
	static ControllerPush	DrumRecActivationSwitch(m_button_push_4, 0x20);
	static ControllerPush	DrumLoopStartStopSwitch(m_button_touch_2, 0x21);
	static ControllerPush	DrumOVDSwitch(m_button_touch_0, 0x27);
	static bool&			DrumGyroCalibButton = m_button_touch_1;

	static ControllerPush	ThRecActivationSwitch(m_button_push_4, 0x23);
	static ControllerPush	ThLoopStartStopSwitch(m_button_touch_2, 0x24);
	static ControllerPush	ThNoteOffSwitch(m_button_touch_1, 0x22);
	static int				ThDistanceOldVal = 0;
	static int				ThAdjustedDistance = 0;

	static bool				ThClFlipOldVal = 0;
	static ControllerRange  ThClFlexController(m_flex, 0x25);
	
	static ControllerSwitch	ClSnareSwitch(m_button_touch_0, 0x27);
	static ControllerSwitch	ClKickSwitch(m_button_touch_1, 0x28);
	static ControllerSwitch	ClTriSwitch(m_button_touch_2, 0x29);
	static ControllerPush	ClBassLoopStart(m_button_push_2, 0x2A);
	static ControllerRange  ClSnowPadArp(m_poti_0, 0x25);

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
		
		if (DrumGyroCalibButton) m_orientation_y.calibrate();
		if (DrumRecActivationSwitch.Actuated()) m_midi_signals.push_back(DrumRecActivationSwitch.Signal(m_channel));
		if (DrumLoopStartStopSwitch.Actuated()) m_midi_signals.push_back(DrumLoopStartStopSwitch.Signal(m_channel));
		break;
	

	case 2: //THEREMIN:
	
		if (ThRecActivationSwitch.Actuated()) m_midi_signals.push_back(ThRecActivationSwitch.Signal(m_channel));
		if (ThLoopStartStopSwitch.Actuated()) m_midi_signals.push_back(ThLoopStartStopSwitch.Signal(m_channel));
		if (ThClFlexController.Changed()) m_midi_signals.push_back(ThClFlexController.Signal(m_channel));
		
		//Note over Distance:
		ThAdjustedDistance = m_distance * 129;
		if (ThAdjustedDistance >= 0 && ThAdjustedDistance <= 3277) ThAdjustedDistance = 27;//4682;
		else if (ThAdjustedDistance >= 3278 && ThAdjustedDistance <= 6553) ThAdjustedDistance = 29;// 7022;
		else if (ThAdjustedDistance >= 6554 && ThAdjustedDistance <= 9830) ThAdjustedDistance = 30;// 8192;
		else if (ThAdjustedDistance >= 9831 && ThAdjustedDistance <= 13106) ThAdjustedDistance = 34;//12871;
		else ThAdjustedDistance = 37;// 16383;
		if (ThAdjustedDistance != ThDistanceOldVal) {
			MidiSignal pitchsignal;
			//pitchsignal.Status(Midi::Status::PitchBend);
			pitchsignal.Status(Midi::Status::NoteOn);
			pitchsignal.Channel(m_channel);
			pitchsignal.Key(ThAdjustedDistance);
			pitchsignal.Velocity((Midi::byte_t)m_poti_2);
			m_midi_signals.push_back(pitchsignal);
			MidiSignal off;
			off.Status(Midi::Status::NoteOff);
			off.Channel(m_channel);
			off.Key(ThDistanceOldVal);
			off.Velocity(0);
			m_midi_signals.push_back(off);
			ThDistanceOldVal = ThAdjustedDistance;
		}
		//StopNote:
		if (ThNoteOffSwitch.Actuated()){
			MidiSignal stopSignal;
			stopSignal.Status(Midi::Status::NoteOff);
			stopSignal.Channel(m_channel);
			stopSignal.Key(ThAdjustedDistance);
			stopSignal.Velocity(0);
			m_midi_signals.push_back(stopSignal);
		}
		//Flip:
		if (m_button_flip != ThClFlipOldVal){
			MidiSignal flipsignal;
			flipsignal.Status(Midi::Status::ControllerChange);
			flipsignal.Channel(m_channel);
			flipsignal.Controller((Midi::Controller) 0x26);
			flipsignal.ControllerValue(m_button_flip * 127);
			ThClFlipOldVal = m_button_flip;
			m_midi_signals.push_back(flipsignal);
		}
		break;


	case 3: //ClipControl:
		
		if (ThClFlexController.Changed()) m_midi_signals.push_back(ThClFlexController.Signal(m_channel));
		if (ClSnowPadArp.Changed()) m_midi_signals.push_back(ClSnowPadArp.Signal(m_channel));
		if (ClBassLoopStart.Actuated()) m_midi_signals.push_back(ClBassLoopStart.Signal(m_channel));
		if (ClSnareSwitch.Switched()) m_midi_signals.push_back(ClSnareSwitch.Signal(m_channel));
		if (ClKickSwitch.Switched()) m_midi_signals.push_back(ClKickSwitch.Signal(m_channel));
		if (ClTriSwitch.Switched()) m_midi_signals.push_back(ClTriSwitch.Signal(m_channel));
		
		//Flip:
		if (m_button_flip != ThClFlipOldVal){
			MidiSignal flipsignal;
			flipsignal.Status(Midi::Status::ControllerChange);
			flipsignal.Channel(m_channel);
			flipsignal.Controller((Midi::Controller) 0x26);
			flipsignal.ControllerValue(m_button_flip * 127);
			ThClFlipOldVal = m_button_flip;
			m_midi_signals.push_back(flipsignal);
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

