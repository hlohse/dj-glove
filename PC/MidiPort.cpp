#include "MidiPort.h"
#include "Formatter.h"
#include <stdexcept>
using namespace std;

#ifdef _WIN32
#include <locale>
#include <codecvt>
#endif

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

#ifdef __linux__
    const int mode = SND_RAWMIDI_SYNC;
    const int status = snd_rawmidi_open(NULL,
                                        &port_,
                                        "virtual",
                                        mode);

    if (status < 0) {
        Close();
        throw runtime_error(Formatter()
            << "Failed to open virtual MIDI port \"" << name << "\""
            << ": " << snd_strerror(status) << "(" << status << ")"); 
    }
#elif _WIN32
	wstring_convert<codecvt_utf8<wchar_t>, wchar_t> converter;
	const wstring name_utf8 = converter.from_bytes(name_);
	const LPCWSTR port_name = name_utf8.c_str();

	port_ = virtualMIDICreatePortEx2(port_name,
                                     MidiPort::Callback,
                                     NULL,
                                     MidiPort::buffer_size,
                                     0);

	if (!IsOpen()) {
		throw runtime_error(Formatter()
            << "Failed to open virtual MIDI port \"" << name << "\""
            << ": " << GetLastError());
	}
#endif
}

void MidiPort::Close()
{
	if (IsOpen()) {
#ifdef __linux__
        snd_rawmidi_close(port_);
#elif _WIN32
		virtualMIDIClosePort(port_);
#endif
		port_ = nullptr;
	}
}

bool MidiPort::IsOpen() const
{
	return port_ != nullptr;
}

void MidiPort::Play(MidiSignal& midi_signal)
{
#ifdef __linux__
    const int status = snd_rawmidi_write(port_,
                                         midi_signal.Bytes(),
                                         midi_signal.NumBytes());

    if (status < 0) {
        throw runtime_error(Formatter()
            << "Failed to play MIDI signal "
            << midi_signal.ToString() << ": "
            << snd_strerror(status) << "(" << status << ")");
    }
#elif _WIN32
	if (!virtualMIDISendData(port_,
                             (LPBYTE) midi_signal.Bytes(),
                             midi_signal.NumBytes()))
    {
		throw runtime_error(Formatter()
            << "Failed to play MIDI signal "
			<< midi_signal.ToString()  << ": " << GetLastError());
	}
#endif
}

#ifdef _WIN32
void CALLBACK MidiPort::Callback(LPVM_MIDI_PORT midiPort,
								 LPBYTE midiDataBytes,
								 DWORD length,
								 DWORD_PTR dwCallbackInstance)
{
	// Do nothing, as we don't intend to receive MIDI data
}
#endif

