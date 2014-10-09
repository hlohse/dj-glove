#include "DjGlove.h"
#include "MidiSignal.h"
#include "ControllerSwitch.h"
#include "ControllerRange.h"
#include "ControllerGyro.h"
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

	static ControllerSwitch	DrumRecActivationSwitch(m_button_push_4, 0x20, change);
	static ControllerSwitch	DrumLoopStartStopSwitch(m_button_touch_2, 0x21, change);
	static ControllerSwitch	DrumOVDSwitch(m_button_touch_0, 0x27, change);
	static bool&			DrumGyroCalibButton = m_button_touch_1;

	static ControllerSwitch	ThRecActivationSwitch(m_button_push_4, 0x23, change);
	static ControllerSwitch	ThLoopStartStopSwitch(m_button_touch_2, 0x24, change);
	static ControllerSwitch	ThNoteOffSwitch(m_button_touch_1, 0x22, change);
	static int				ThDistanceOldVal = 0;
	static int				ThAdjustedDistance = 0;

	static bool				ThClFlipOldVal = 0;
	static ControllerRange  ThClFlexController(m_flex, 0x25);
	
	static ControllerSwitch	ClSnareSwitch(m_button_touch_0, 0x27, press);
	static ControllerSwitch	ClKickSwitch(m_button_touch_1, 0x28, press);
	static ControllerSwitch	ClTriSwitch(m_button_touch_2, 0x29, press);
	static ControllerSwitch	ClBassLoopStart(m_button_push_2, 0x2A, press);
	static ControllerRange  ClSnowPadArp(m_poti_0, 0x25);

	static ControllerRange  ThDistanceController(ThAdjustedDistance, 0, {
        ControllerRange::partition_t(0,     3277,  57),
        ControllerRange::partition_t(3278,  6553,  59),
        ControllerRange::partition_t(6554,  9830,  60),
        ControllerRange::partition_t(9831,  13106, 64),
        ControllerRange::partition_t(13107, 16383, 67)
    });

    static ControllerGyro GyroEffectControl0(m_button_touch_0, m_orientation_x, 0x2C);
    static ControllerGyro GyroEffectControl1(m_button_touch_1, m_orientation_x, 0x2D);
    static ControllerGyro GyroEffectControl2(m_button_touch_2, m_orientation_x, 0x2E);

	switch (m_program) {

	case 1: //DRUMS
		
		//Hit:
		if (m_hit_intensity > 0){
			int hit_note = 36;
			if (m_distance > 70) hit_note += 4;
			if (m_orientation_y.Degree() > 0) hit_note += 2;
			if (m_button_touch_3) hit_note += 1;
            Register({Midi::Status::NoteOn,  m_channel, hit_note, m_hit_intensity});
            Register({Midi::Status::NoteOff, m_channel, hit_note, m_hit_intensity});
		}
		
		if (DrumGyroCalibButton) m_orientation_y.calibrate();
		if (DrumRecActivationSwitch.Changed()) Register(DrumRecActivationSwitch.Signal(m_channel));
		if (DrumLoopStartStopSwitch.Changed()) Register(DrumLoopStartStopSwitch.Signal(m_channel));
		break;
	

	case 2: //THEREMIN:
	
		if (ThRecActivationSwitch.Changed()) Register(ThRecActivationSwitch.Signal(m_channel));
		if (ThLoopStartStopSwitch.Changed()) Register(ThLoopStartStopSwitch.Signal(m_channel));
		if (ThClFlexController.Changed()) Register(ThClFlexController.Signal(m_channel));
		
		//Note over Distance:
		ThAdjustedDistance = m_distance * 129;
		if (ThDistanceController.Changed()) {
			Register({ Midi::Status::NoteOn,  m_channel, ThDistanceController.PartitionValue(), m_poti_2});
			Register({ Midi::Status::NoteOff, m_channel, ThDistanceOldVal, 0 });
			ThDistanceOldVal = ThDistanceController.PartitionValue();
		}
		//StopNote:
		if (ThNoteOffSwitch.Changed()){
			Register({ Midi::Status::NoteOff, m_channel, ThDistanceController.PartitionValue(), 0 });
		}
		//Flip:
		if (m_button_flip != ThClFlipOldVal){
			Register({ Midi::Status::ControllerChange, m_channel, 0x26, m_button_flip * 127 });
			ThClFlipOldVal = m_button_flip;
		}
		break;


	case 3: //ClipControl:
		
		if (ThClFlexController.Changed()) Register(ThClFlexController.Signal(m_channel));
		if (ClSnowPadArp.Changed()) Register(ClSnowPadArp.Signal(m_channel));
		if (ClBassLoopStart.Changed()) Register(ClBassLoopStart.Signal(m_channel));
		if (ClSnareSwitch.Changed()) Register(ClSnareSwitch.Signal(m_channel));
		if (ClKickSwitch.Changed()) Register(ClKickSwitch.Signal(m_channel));
		if (ClTriSwitch.Changed()) Register(ClTriSwitch.Signal(m_channel));
		
		//Flip:
		if (m_button_flip != ThClFlipOldVal){
			Register({ Midi::Status::ControllerChange, m_channel, 0x26, m_button_flip * 127 });
			ThClFlipOldVal = m_button_flip;
		}
		break;

    case 4: // GyroEffectControl 
        if (GyroEffectControl0.Changed()) {
            Register(GyroEffectControl0.Signal(m_channel));
        }
        if (GyroEffectControl1.Changed()) {
            Register(GyroEffectControl1.Signal(m_channel));
        }
        if (GyroEffectControl2.Changed()) {
            Register(GyroEffectControl2.Signal(m_channel));
        }
		//Flip:
		if (m_button_flip != ThClFlipOldVal){
			Register({ Midi::Status::ControllerChange, m_channel, 0x26, m_button_flip * 127 });
			ThClFlipOldVal = m_button_flip;
		}
        break;
	}
}

void DjGlove::Register(const MidiSignal& midi_signal)
{
    m_midi_signals.push_back(midi_signal);
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

