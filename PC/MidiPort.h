#ifndef DJ_GLOVE_PC_MIDI_PORT_H_
#define DJ_GLOVE_PC_MIDI_PORT_H_

#include "Export.h"
#include "MidiSignal.h"
#include <string>

#ifdef __linux__
#elif _WIN32
#include "teVirtualMIDI.h"
#endif

class EXPORT MidiPort {
public:
#ifdef __linux__
    using port_type = int*;
#elif _WIN32
    using port_type = LPVM_MIDI_PORT;
#endif
	
    MidiPort();
	~MidiPort();

	void Open(const std::string& name);
	void Close();
	bool IsOpen() const;

	void Play(MidiSignal& midi_signal);

private:
	static const int buffer_size = 0x1FFFE;

	std::string name_;
	port_type port_;

#ifdef _WIN32
	static void CALLBACK Callback(LPVM_MIDI_PORT midiPort,
								  LPBYTE midiDataBytes,
								  DWORD length,
								  DWORD_PTR dwCallbackInstance);
#endif
};

#endif // DJ_GLOVE_PC_MIDI_PORT_H_
