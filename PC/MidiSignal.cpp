#include "MidiSignal.h"
using namespace std;

MidiSignal::MidiSignal(const string& bytes)
:	MidiSignal(0, 0, 0)
{
	if (bytes.length() == NumBytes()) {
		for (unsigned int i = 0; i < bytes.length(); ++i) {
			bytes_[i] = *((MidiSignal::Byte*) bytes.c_str()[i]);
		}
	}
}

MidiSignal::MidiSignal(const MidiSignal::Byte byte_0,
					   const MidiSignal::Byte byte_1,
					   const MidiSignal::Byte byte_2)
{
	bytes_[0] = byte_0;
	bytes_[1] = byte_1;
	bytes_[2] = byte_2;
}

MidiSignal::~MidiSignal()
{
}

MidiSignal::Byte* MidiSignal::Bytes()
{
	return bytes_;
}

int MidiSignal::NumBytes() const
{
	return MidiSignal::num_bytes;
}
