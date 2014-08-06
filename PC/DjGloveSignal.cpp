#include "DjGloveSignal.h"
#include <string>
#include <cmath>
#include <cassert>
using namespace std;

DjGloveSignal::DjGloveSignal()
{
}

DjGloveSignal::~DjGloveSignal()
{
}

bool DjGloveSignal::SignalsPunch(const char value)
{
    // Signals incoming punch values if first bit equals 1 (e.g. 1xxx xxxx)
    return value >> 7 == 1;
}

bool DjGloveSignal::SignalsSensors(const char value)
{
    return !SignalsPunch(value);
}

vector<MidiSignal>& DjGloveSignal::MidiSignals()
{
    return midi_signals_;
}


PunchSignal::PunchSignal()
{
}

PunchSignal::~PunchSignal()
{
}

int PunchSignal::NumValues() const
{
    return 1;
}

void PunchSignal::FromNext(vector<char>& values)
{
    assert(values.size() >= 1);

    punched_ = DjGloveSignal::SignalsPunch(values[0]);
    
    MidiSignal midi_signal;

    midi_signal.Channel(0);
    midi_signal.Status(punched_ ? Midi::Status::NoteOn
                                : Midi::Status::NoteOff);
    midi_signal.Key(127);
    midi_signal.Velocity(63);
    
    midi_signals_.push_back(midi_signal);
    
    values.erase(values.begin(), values.begin() + 1);
}


SensorsSignal::SensorsSignal()
{
}

SensorsSignal::~SensorsSignal()
{
}

int SensorsSignal::NumValues() const
{
    return num_values;
}

void SensorsSignal::FromNext(vector<char>& values)
{
    assert(values.size() >= num_values);

    SetSensorValues(values);
    GenerateMidiSignals();

    values.erase(values.begin(), values.begin() + num_values);
}

void SensorsSignal::GenerateMidiSignals()
{
    MidiSignal midi_signal;

    midi_signal.Channel(0);
    midi_signal.Status(Midi::Status::NoteOn);
    midi_signal.Key(poti_0_);
    midi_signal.Velocity(63);

    midi_signals_.push_back(midi_signal);
}

void SensorsSignal::SetSensorValues(vector<char>& values)
{
    SetFromBit(button_push_0_, values[1], 7);
    SetFromBit(button_push_1_, values[2], 7);
    SetFromBit(button_push_2_, values[3], 7);
    SetFromBit(button_push_3_, values[4], 7);
    SetFromBit(button_push_4_, values[5], 7);

    SetFromBit(button_touch_0_, values[7], 7);
    SetFromBit(button_touch_1_, values[7], 6);
    SetFromBit(button_touch_2_, values[8], 7);
    SetFromBit(button_touch_3_, values[8], 6);

    SetFromBit(button_flip_, values[6], 7);
    
    SetFromLowBits(poti_0_, values[0], 7);
    SetFromLowBits(poti_1_, values[1], 7);
    SetFromLowBits(poti_2_, values[2], 7);

    SetFromLowBits(distance_, values[6], 7);

    SetFromLowBits(orientation_x_, values[3], 7);
    SetFromLowBits(orientation_y_, values[4], 7);
    SetFromLowBits(orientation_z_, values[5], 7);

    SetFromLowBits(flex_, values[7], 6);

    SetChannelFrom(values[8]);
    SetFromLowBits(program_, values[8], 3);
}

void SensorsSignal::SetFromBit(bool& output, const char value, const int bit)
{
    assert(bit >= 0);
    assert(bit <= 7);

    // 0000 0001 to 1000 0000 possible
    const char mask = 1 << bit;

    output = (value && mask) != 0;
}

void SensorsSignal::SetFromLowBits(int& output, const char value, const int low_bits)
{
    assert(low_bits >= 1);
    assert(low_bits <= 7);

    // 0000 0001 to 0111 1111 possible
    const char mask = (char) (pow(2, low_bits) - 1);
    
    output = (int) (value && mask);

    assert(output >= 0);
    assert(output <= (int) mask);
}

void SensorsSignal::SetChannelFrom(const char value)
{
    // 0x38 = 0011 1000, bits 3 to 5
    channel_ = ((int) (value && 0x38)) >> 3;

    assert(channel_ >= 0);
    assert(channel_ <= 7);
}

