#include "MidiChannel.h"

MidiChannel::MidiChannel(const int channel)
{
	Status(Midi::NoteOff);
	Key(0);
	Velocity(0);
    Channel(channel);
}

MidiChannel::MidiChannel()
{
	Status(Midi::NoteOff);
	Key(0);
	Velocity(0);
}

MidiChannel::~MidiChannel()
{
}

char* MidiChannel::Signal()
{
    return (char*) data_;
}

int MidiChannel::SignalLength() const
{
    return current_data_length_;
}

void MidiChannel::Channel(const int channel) 
{
    data_[0] = Midi::CombineStatusChannel(Status(), (Midi::byte_t) channel);
}

int MidiChannel::Channel() const
{
	Midi::Status status;
	Midi::byte_t channel;
    Midi::SplitStatusChannel(data_[0], status, channel);
    return (int) channel;
}

void MidiChannel::Status(const Midi::Status status)
{
    data_[0] = Midi::CombineStatusChannel(status, (Midi::byte_t) Channel());

    switch (status) {
        case Midi::ProgramChange:
        case Midi::ChannelPressure:
            current_data_length_ = 2;
            break;
        default:
            current_data_length_ = 3;
            break;
    }
}

Midi::Status MidiChannel::Status() const
{
	Midi::Status status;
	Midi::byte_t channel;
    Midi::SplitStatusChannel(data_[0], status, channel);
    return status;
}

void MidiChannel::Value(const int value, const int data_index)
{
    data_[data_index] = Midi::Limit((Midi::byte_t) value, Midi::Value);
}

int MidiChannel::Value(const int data_index) const
{
    return (int) data_[data_index];
}

void MidiChannel::Key(const int key)                            { Value(key, 1); }
int  MidiChannel::Key() const                                   { return Value(1); }

void MidiChannel::Velocity(const int velocity)                  { Value(velocity, 2); }
int  MidiChannel::Velocity() const                              { return Value(2); }

void MidiChannel::Pressure(const int pressure)                  { Value(pressure, 2); }
int  MidiChannel::Pressure() const                              { return Value(2); }

void MidiChannel::Controller(const Midi::Controller controller) { Value(controller, 1); }
Midi::Controller MidiChannel::Controller() const                { return (Midi::Controller) Value(1); }

void MidiChannel::ControllerValue(const int value)              { Value(value, 2); }
int  MidiChannel::ControllerValue() const                       { return Value(2); }

void MidiChannel::Program(const int preset)                     { Value(preset, 1); }
int  MidiChannel::Program() const                               { return Value(1); }

void MidiChannel::ChannelPressure(const int channel_pressure)   { Value(channel_pressure, 1); }
int  MidiChannel::ChannelPressure() const                       { return Value(1); }
    
void MidiChannel::PitchBend(const int pitch_bend)
{
    Midi::SplitPitchBend((Midi::word_t) pitch_bend, data_[1], data_[2]);
}

int MidiChannel::PitchBend() const
{
    return (int) Midi::CombinePitchBend(data_[1], data_[2]);
}

