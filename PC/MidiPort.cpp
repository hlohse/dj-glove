#include "MidiPort.h"
#include "Formatter.h"
#include <stdexcept>
using namespace std;

#ifdef _WIN32
#include <locale>
#include <codecvt>
#endif

MidiPort::MidiPort()
:	m_name(""),
	m_port(nullptr)
{
}

MidiPort::~MidiPort()
{
	Close();
}

void MidiPort::Open(const string& name)
{
	Close();

#ifdef __linux__
    const int mode = SND_RAWMIDI_SYNC;
    snd_rawmidi_t* handle = NULL;

    const int status = snd_rawmidi_open(&handle,
                                        &m_port,
                                        "virtual",
                                        mode);

    if (status < 0) {
        Close();
        throw runtime_error(Formatter()
            << "Failed to open virtual MIDI port \"" << name << "\""
            << ": " << snd_strerror(status) << "(" << status << ")"); 
    }

    m_name = string(snd_rawmidi_name(handle));
#elif _WIN32
	wstring_convert<codecvt_utf8<wchar_t>, wchar_t> converter;
	const wstring m_nameutf8 = converter.from_bytes(name);
	const LPCWSTR m_portname = m_nameutf8.c_str();

	m_port = virtualMIDICreatePortEx2(m_portname,
                                     MidiPort::Callback,
                                     NULL,
                                     MidiPort::buffer_size,
                                     0);

	if (!IsOpen()) {
		throw runtime_error(Formatter()
            << "Failed to open virtual MIDI port \"" << name << "\""
            << ": " << GetLastError());
	}
	
    m_name = name;
#endif
}

void MidiPort::Close()
{
	if (IsOpen()) {
#ifdef __linux__
        snd_rawmidi_close(m_port);
#elif _WIN32
		virtualMIDIClosePort(m_port);
#endif
		m_port = nullptr;
	}
}

bool MidiPort::IsOpen() const
{
	return m_port != nullptr;
}

void MidiPort::Play(MidiSignal& midi_signal)
{
#ifdef __linux__
    const int status = snd_rawmidi_write(m_port,
                                         midi_signal.Signal(),
                                         midi_signal.SignalLength());

    if (status < 0) {
        throw runtime_error(Formatter()
            << "Failed to play MIDI signal "
            << midi_signal.ToString() << ": "
            << snd_strerror(status) << "(" << status << ")");
    }
#elif _WIN32
	if (!virtualMIDISendData(m_port,
                             (LPBYTE) midi_signal.Signal(),
                             midi_signal.SignalLength()))
    {
		throw runtime_error(Formatter()
            << "Failed to play MIDI signal "
			<< midi_signal.ToString()  << ": " << GetLastError());
	}
#endif
}

string MidiPort::Name() const
{
    return m_name;
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

