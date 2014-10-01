#ifndef DJ_GLOVE_PC_MIDI_PORT_H_
#define DJ_GLOVE_PC_MIDI_PORT_H_

#include "Export.h"
#include "MidiSignal.h"
#include <string>

#ifdef __linux__
#include <alsa/asoundlib.h>
#elif _WIN32
#include "teVirtualMIDI.h"
#endif

class EXPORT MidiPort {
public:
#ifdef __linux__
    using port_t = snd_rawmidi_t*;
#elif _WIN32
    using port_t = LPVM_MIDI_PORT;
#endif
	
    MidiPort();
	~MidiPort();

	void Open(const std::string& name);
	void Close();
	bool IsOpen() const;

	void Play(MidiSignal& midi_signal);

    std::string Name() const;

private:
	std::string m_name;
	port_t      m_port;

#ifdef _WIN32
	static const int buffer_size = 0x1FFFE;

	static void CALLBACK Callback(LPVM_MIDI_PORT midiPort,
								  LPBYTE midiDataBytes,
								  DWORD length,
								  DWORD_PTR dwCallbackInstance);
#endif
};

#endif // DJ_GLOVE_PC_MIDI_PORT_H_
