#include "MidiSignal.h"
#include <sstream>
#include <iomanip>
using namespace std;

MidiSignal::MidiSignal(const string& bytes)
:	MidiSignal(0, 0, 0)
{
	if (bytes.length() == NumBytes()) {
		for (unsigned int i = 0; i < bytes.length(); ++i) {
			bytes_[i] = *((MidiSignal::Byte*) bytes.c_str()[i]);
		}
	}

	num_bytes_ = bytes.length();
}

MidiSignal::MidiSignal(const MidiSignal::Byte byte_0,
					   const MidiSignal::Byte byte_1,
					   const MidiSignal::Byte byte_2)
:	num_bytes_(3)
{
	bytes_[0] = byte_0;
	bytes_[1] = byte_1;
	bytes_[2] = byte_2;
}

MidiSignal::MidiSignal(const MidiSignal::Byte byte_0,
					   const MidiSignal::Byte byte_1)
:	num_bytes_(2)
{
	bytes_[0] = byte_0;
	bytes_[1] = byte_1;
	bytes_[2] = 0;
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
	return num_bytes_;
}

string MidiSignal::ToString() const
{
	ostringstream result;

	result << hex << bytes_[0] << "|" << bytes_[1];
		
	if (num_bytes_ == 3) {
		result << "|" << bytes_[2];
	}

	return result.str();
}
