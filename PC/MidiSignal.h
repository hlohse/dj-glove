/*      MidiSignal
 *
 *  Provides functionality to describe MIDI signals. Controls range from key
 *  and velocity to program presets etc.
 *  The resulting signal bytes can be retrieved via Signal and SignalLength.
 *
 *  Example, create middle C on channel 1
 *
 *      MidiSignal midi_signal;
 *
 *      midi_signal.Channel(1);
 *      midi_signal.Status(Midi::NoteOn);
 *      midi_signal.Key(60); // Middle C
 *      midi_signal.Velocity(63);
 */

#ifndef DJ_GLOVE_PC_MIDI_SIGNAL_H_
#define DJ_GLOVE_PC_MIDI_SIGNAL_H_

#include "Midi.h"
#include <string>

class MidiSignal {
public:
    MidiSignal();
    ~MidiSignal();

    char* Signal();
    int SignalLength() const;

    void Channel(const int channel);
    int Channel() const;

    void Status(const Midi::Status status);
    Midi::Status Status() const;

    void Key(const int key);
    int Key() const;

    void Velocity(const int velocity);
    int Velocity() const;

    void Pressure(const int pressure);
    int Pressure() const;

    void Controller(const Midi::Controller controller);
    Midi::Controller Controller() const;

    void ControllerValue(const int value);
    int ControllerValue() const;

    void Program(const int preset);
    int Program() const;

    void ChannelPressure(const int channel_pressure);
    int ChannelPressure() const;
    
    void PitchBend(const int pitch_bend);
    int PitchBend() const;

    std::string ToString() const;

private:
    Midi::byte_t m_data[3];
    int m_data_length;

    void Value(const int value, const int m_dataindex);
    int  Value(const int m_dataindex) const;
};

#endif // DJ_GLOVE_PC_MIDI_SIGNAL_H_

