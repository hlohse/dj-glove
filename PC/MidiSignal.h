#ifndef DJ_GLOVE_PC_MIDI_SIGNAL_H_
#define DJ_GLOVE_PC_MIDI_SIGNAL_H_

#include "Export.h"
#include <string>

class EXPORT MidiSignal {
public:
	using Byte = unsigned char;

	MidiSignal(const std::string& bytes);
	~MidiSignal();

	Byte* Bytes();
	int NumBytes() const;

	std::string ToString() const;

private:
	Byte bytes_[3];
	int num_bytes_;
};

#endif // DJ_GLOVE_PC_MIDI_SIGNAL_H_
