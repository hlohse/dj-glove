#ifndef DJ_GLOVE_PC_MIDI_SIGNAL_H_
#define DJ_GLOVE_PC_MIDI_SIGNAL_H_

#include "Export.h"
#include <string>

class EXPORT MidiSignal {
public:
	using Byte = unsigned char;

	MidiSignal(const std::string& bytes);
	MidiSignal(const Byte byte_0,
			   const Byte byte_1,
			   const Byte byte_2);
	MidiSignal(const Byte byte_0,
			   const Byte byte_1);
	~MidiSignal();

	Byte* Bytes();
	int NumBytes() const;

	std::string ToString() const;

private:
	Byte bytes_[3];
	int num_bytes_;
};

#endif // DJ_GLOVE_PC_MIDI_SIGNAL_H_
