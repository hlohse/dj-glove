#include "MidiSignal.h"
#include <sstream>
#include <iomanip>
using namespace std;

MidiSignal::MidiSignal(const string& bytes)
{
	for (unsigned int i = 0; i < bytes.length(); ++i) {
		bytes_[i] = ((MidiSignal::Byte*) bytes.c_str())[i] - 1;
	}

	num_bytes_ = bytes.length();
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

	result << hex << "[ " << (int) bytes_[0] << " " << (int) bytes_[1];
		
	if (num_bytes_ == 3) {
		result << " " << (int) bytes_[2];
	}

	result << " ]";

	return result.str();
}
