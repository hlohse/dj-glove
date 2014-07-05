#include "MidiPort.h"
#include "Formatter.h"
#include <stdexcept>
#include <locale>
#include <codecvt>
using namespace std;

MidiPort::MidiPort(const string& name)
:	MidiPort()
{
	Open(name);
}

MidiPort::MidiPort()
:	name_(""),
	port_(nullptr)
{
}

MidiPort::~MidiPort()
{
	Close();
}

void MidiPort::Open(const string& name)
{
	Close();
	name_ = name;

	wstring_convert<codecvt_utf8<wchar_t>, wchar_t> converter;
	const wstring name_utf8 = converter.from_bytes(name_);
	const LPCWSTR port_name = name_utf8.c_str();

	port_ = virtualMIDICreatePortEx2(port_name, MidiPort::Callback, NULL, MidiPort::buffer_size, 0);

	if (!IsOpen()) {
		throw runtime_error(Formatter() << "Failed to open virtual MIDI port \""
			<< name << "\": " << GetLastError());
	}
}

void MidiPort::Close()
{
	if (IsOpen()) {
		virtualMIDIClosePort(port_);
		port_ = nullptr;
	}
}

bool MidiPort::IsOpen() const
{
	return port_ != nullptr;
}

void MidiPort::Play(MidiSignal& midi_signal)
{
	if (!virtualMIDISendData(port_, (LPBYTE) midi_signal.Bytes(), midi_signal.NumBytes())) {
		throw runtime_error(Formatter() << "Failed to play MIDI signal "
			<< midi_signal.ToString()  << ": " << GetLastError());
	}
}

void CALLBACK MidiPort::Callback(LPVM_MIDI_PORT midiPort,
								 LPBYTE midiDataBytes,
								 DWORD length,
								 DWORD_PTR dwCallbackInstance)
{
	// Do nothing, as we don't intend to receive MIDI data
}