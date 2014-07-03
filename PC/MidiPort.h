#ifndef DJ_GLOVE_PC_MIDI_PORT_H_
#define DJ_GLOVE_PC_MIDI_PORT_H_

#include "Export.h"
#include "MidiSignal.h"
#include "teVirtualMIDI.h"
#include <string>

class EXPORT MidiPort {
public:
	MidiPort(const std::string& name);
	MidiPort();
	~MidiPort();

	void Open(const std::string& name);
	void Close();
	bool IsOpen() const;

	void Play(MidiSignal& midi_signal);

private:
	static const int buffer_size = 0x1FFFE;

	std::string name_;
	LPVM_MIDI_PORT port_;

	static void CALLBACK Callback(LPVM_MIDI_PORT midiPort,
								  LPBYTE midiDataBytes,
								  DWORD length,
								  DWORD_PTR dwCallbackInstance);
};

#endif // DJ_GLOVE_PC_MIDI_PORT_H_
