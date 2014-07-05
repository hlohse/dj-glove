/*      MidiChannel
 *
 *  Provides functionality to control MIDI signals on a single channel. Controls
 *  range from key and velocity to program presets etc.
 *  The resulting signal bytes can be retrieved via Signal.
 *
 *  Example, send middle C on channel 1 via Serial:
 *
 *      MidiChannel channel(1);
 *
 *      channel.Status(Midi::NoteOn);
 *      channel.Key(60); // Middle C
 *      channel.Velocity(64);
 *
 *      Serial.write(channel.Signal());
 */

#ifndef DJ_GLOVE_ARDUINO_LIBRARY_MIDI_CHANNEL_H_
#define DJ_GLOVE_ARDUINO_LIBRARY_MIDI_CHANNEL_H_

#include <Midi.h>

class MidiChannel {
public:
    MidiChannel(const int channel);
    MidiChannel();
    ~MidiChannel();

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

private:
    Midi::byte_t data_[3];
    int current_data_length_;

    void Value(const int value, const int data_index);
    int  Value(const int data_index) const;
};

#endif // DJ_GLOVE_ARDUINO_LIBRARY_MIDI_CHANNEL_H_

