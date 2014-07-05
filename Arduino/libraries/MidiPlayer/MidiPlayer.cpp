#include "MidiPlayer.h"
#include <Bluetooth.h>
#include <Arduino.h>
#include <SoftwareSerial.h>

MidiPlayer::MidiPlayer()
{
    for (int i = 0; i < num_channels; i++) {
        channels_[i].Channel(i);
    }
}

MidiPlayer::~MidiPlayer()
{
}

MidiChannel& MidiPlayer::operator()(const int channel)
{
    return Channel(channel);
}

MidiChannel& MidiPlayer::Channel(const int channel)
{
    if (channel < 0) {
        return channels_[0];
    }
    
    if (channel >= num_channels) {
        return channels_[num_channels - 1];
    }
    
    return channels_[channel];
}

void MidiPlayer::Play(const int channel)
{
    Play(Channel(channel));
}

void MidiPlayer::Play(MidiChannel& midi_channel)
{
    const uint8_t length = (uint8_t) midi_channel.SignalLength();
    uint8_t* signal = (uint8_t*) midi_channel.Signal();

    Bluetooth.write(length);

    for (int i = 0; i < length; i++) {
        Bluetooth.write(*signal);
        signal++;
    }
}

