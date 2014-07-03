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
	~MidiSignal();

	Byte* Bytes();
	int NumBytes() const;

private:
	static const int num_bytes = 3;

	Byte bytes_[num_bytes];
};

#endif // DJ_GLOVE_PC_MIDI_SIGNAL_H_
