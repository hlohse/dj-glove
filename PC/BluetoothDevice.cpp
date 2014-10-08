#include "BluetoothDevice.h"
#include <sstream>
#include <cstdint>
using namespace std;

shared_ptr<BluetoothDevice> BluetoothDevice::Arduino()
{
    static shared_ptr<BluetoothDevice> arduino = make_shared<BluetoothDevice>(
        "Arduino", "98:d3:31:b3:0a:25", 1);
    return arduino;
}

BluetoothDevice::BluetoothDevice(const string& name,
                                 const string& address,
                                 const int channel)
:   m_name(name),
    m_address(address),
    m_channel(channel),
	m_is_valid(false)
{
	m_is_valid |= (channel >= 1 && channel <= 12);

#ifdef __linux__
	m_is_valid &= str2ba(m_address.c_str(), &m_socket_address.rc_bdaddr) == 0;

    if (m_is_valid) {
        m_socket_address.rc_family  = AF_BLUETOOTH;
        m_socket_address.rc_channel = (uint8_t) m_channel;
    }
#elif _WIN32
	m_is_valid &= StringToAddress(m_address, m_socket_address.btAddr) == 0;

	if (m_is_valid) {
		m_socket_address.addressFamily  = AF_BTH;
		m_socket_address.serviceClassId = GUID_NULL;
		m_socket_address.port =			  m_channel;
	}
#endif
}

#ifdef _WIN32
// http://social.msdn.microsoft.com/Forums/en-US/9d47f33c-c848-4728-ae36-f251623daf17/bluetooth-101-wsastringtoaddress
int BluetoothDevice::StringToAddress(IN const string& m_addressstring,
									 OUT BTH_ADDR& address)
{
	BTH_ADDR temp_address;
	ULONG m_addressdata[6];

	if (m_addressstring.empty()) {
		return -1;
	}

	if (sscanf_s(m_addressstring.c_str(),
		"%02x:%02x:%02x:%02x:%02x:%02x",
		&m_addressdata[0],
		&m_addressdata[1],
		&m_addressdata[2],
		&m_addressdata[3],
		&m_addressdata[4],
		&m_addressdata[5]) != 6)
	{
		return -1;
	}

	address = 0;
	for (int i = 0; i < 6; ++i) {
		// Extract data from the first 8 lower bits.
		temp_address = (BTH_ADDR) (m_addressdata[i] & 0xFF);

		// Push 8 bits to the left
		address = (address << 8) + temp_address;
	}

	return 0;
}
#endif

BluetoothDevice::~BluetoothDevice()
{
}

bool BluetoothDevice::IsValid() const
{
	return m_is_valid;
}

string BluetoothDevice::ToString() const
{
	ostringstream result;

	result << "BluetoothDevice"
		<< " \"" << m_name << "\""
		<< " @ " << m_address
		<< " on channel " << m_channel;

	return result.str();
}

BluetoothDevice::SocketAddress BluetoothDevice::GetSocketAddress() const
{
    return m_socket_address;
}

