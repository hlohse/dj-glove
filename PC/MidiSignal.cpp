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
    return (char*) data_;
}

int MidiSignal::SignalLength() const
{
    return current_data_length_;
}

void MidiSignal::Channel(const int channel) 
{
    data_[0] = Midi::CombineStatusChannel(Status(), (Midi::byte_t) channel);
}

int MidiSignal::Channel() const
{
	Midi::Status status;
	Midi::byte_t channel;
    Midi::SplitStatusChannel(data_[0], status, channel);
    return (int) channel;
}

void MidiSignal::Status(const Midi::Status status)
{
    data_[0] = Midi::CombineStatusChannel(status, (Midi::byte_t) Channel());

    switch (status) {
        case Midi::Status::ProgramChange:
        case Midi::Status::ChannelPressure:
            current_data_length_ = 2;
            break;
        default:
            current_data_length_ = 3;
            break;
    }
}

Midi::Status MidiSignal::Status() const
{
	Midi::Status status;
	Midi::byte_t channel;
    Midi::SplitStatusChannel(data_[0], status, channel);
    return status;
}

void MidiSignal::Value(const int value, const int data_index)
{
    data_[data_index] = Midi::Limit((Midi::byte_t) value, Midi::ValueType::Value);
}

int MidiSignal::Value(const int data_index) const
{
    return (int) data_[data_index];
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
    Midi::SplitPitchBend((Midi::word_t) pitch_bend, data_[1], data_[2]);
}

int MidiSignal::PitchBend() const
{
    return (int) Midi::CombinePitchBend(data_[1], data_[2]);
}

string MidiSignal::ToString() const
{
    ostringstream result;

    result << "[ " << data_[0] << " " << data_[1];

    if (SignalLength() == 3) {
        result << " " << data_[2];
    }

    result << "]";

    return result.str();
}

