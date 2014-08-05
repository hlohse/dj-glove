#include "DjGloveSignal.h"
#include <string>
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
    return value && 0x80;
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
    midi_signals_.reserve(1);
}

PunchSignal::~PunchSignal()
{
}

int PunchSignal::NumValues() const
{
    // TODO: Adjust accordingly
    return 3;
}

void PunchSignal::FromNext(vector<char>& values)
{
    string signal_bytes;
    
    signal_bytes.push_back(values[0]);
    signal_bytes.push_back(values[1]);
    signal_bytes.push_back(values[2]);

    midi_signals_.push_back(MidiSignal(signal_bytes));
    values.erase(values.begin(), values.begin() + 3);
}


SensorsSignal::SensorsSignal()
{
    midi_signals_.reserve(num_midi_signals);
}

SensorsSignal::~SensorsSignal()
{
}

int SensorsSignal::NumValues() const
{
    // TODO: Adjust accordingly
    return num_midi_signals * 3;
}

void SensorsSignal::FromNext(vector<char>& values)
{
    for (int i = 0; i < num_midi_signals; ++i) {
        string signal_bytes;
        
        signal_bytes.push_back(values[0]);
        signal_bytes.push_back(values[1]);
        signal_bytes.push_back(values[2]);

        midi_signals_.push_back(MidiSignal(signal_bytes));
        values.erase(values.begin(), values.begin() + 3);
    }
}

