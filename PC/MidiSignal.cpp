#include "MidiSignal.h"
#include <sstream>
using namespace std;

MidiSignal::MidiSignal()
{
    Channel(0);
	Status(Midi::Status::NoteOff);
	Key(0);
	Velocity(0);
}

MidiSignal::~MidiSignal()
{
}

char* MidiSignal::Signal()
{
    return (char*) m_data;
}

int MidiSignal::SignalLength() const
{
    return m_data_length;
}

void MidiSignal::Channel(const int channel) 
{
    m_data[0] = Midi::CombineStatusChannel(Status(), (Midi::byte_t) channel);
}

int MidiSignal::Channel() const
{
	Midi::Status status;
	Midi::byte_t channel;
    Midi::SplitStatusChannel(m_data[0], status, channel);
    return (int) channel;
}

void MidiSignal::Status(const Midi::Status status)
{
    m_data[0] = Midi::CombineStatusChannel(status, (Midi::byte_t) Channel());

    switch (status) {
        case Midi::Status::ProgramChange:
        case Midi::Status::ChannelPressure:
            m_data_length = 2;
            break;
        default:
            m_data_length = 3;
            break;
    }
}

Midi::Status MidiSignal::Status() const
{
	Midi::Status status;
	Midi::byte_t channel;
    Midi::SplitStatusChannel(m_data[0], status, channel);
    return status;
}

void MidiSignal::Value(const int value, const int m_dataindex)
{
    m_data[m_dataindex] = Midi::Limit((Midi::byte_t) value, Midi::ValueType::Value);
}

int MidiSignal::Value(const int m_dataindex) const
{
    return (int) m_data[m_dataindex];
}

void MidiSignal::Key(const int key)                            { Value(key, 1); }
int  MidiSignal::Key() const                                   { return Value(1); }

void MidiSignal::Velocity(const int velocity)                  { Value(velocity, 2); }
int  MidiSignal::Velocity() const                              { return Value(2); }

void MidiSignal::Pressure(const int pressure)                  { Value(pressure, 2); }
int  MidiSignal::Pressure() const                              { return Value(2); }

void MidiSignal::Controller(const Midi::Controller controller)
{
    Value(static_cast<int>(controller), 1);
}

Midi::Controller MidiSignal::Controller() const                { return (Midi::Controller) Value(1); }

void MidiSignal::ControllerValue(const int value)              { Value(value, 2); }
int  MidiSignal::ControllerValue() const                       { return Value(2); }

void MidiSignal::Program(const int preset)                     { Value(preset, 1); }
int  MidiSignal::Program() const                               { return Value(1); }

void MidiSignal::ChannelPressure(const int channel_pressure)   { Value(channel_pressure, 1); }
int  MidiSignal::ChannelPressure() const                       { return Value(1); }
    
void MidiSignal::PitchBend(const int pitch_bend)
{
    Midi::SplitPitchBend((Midi::word_t) pitch_bend, m_data[1], m_data[2]);
}

int MidiSignal::PitchBend() const
{
    return (int) Midi::CombinePitchBend(m_data[1], m_data[2]);
}

string MidiSignal::ToString() const
{
    ostringstream result;

    result << "[ " << (int) m_data[0] << " " << (int) m_data[1];

    if (SignalLength() == 3) {
        result << " " << (int) m_data[2];
    }

    result << " ]";

    return result.str();
}

