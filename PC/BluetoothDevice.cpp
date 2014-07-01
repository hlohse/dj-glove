#include "BluetoothDevice.h"
#include <sstream>
#include <cstdint>
using namespace std;

const BluetoothDevice BluetoothDevice::Arduino("Arduino", "98:D3:31:B3:0A:25", 1);

BluetoothDevice::BluetoothDevice(const string& name,
                                 const string& address,
                                 const int channel)
:   name_(name),
    address_(address),
    channel_(channel),
	is_valid_(false)
{
	is_valid_ |= (channel >= 1 && channel <= 12);

#ifdef __linux__
	is_valid_ &= str2ba(address_.c_str(), &socket_address_.rc_bdaddr) == 0;

    if (is_valid_) {
        socket_address_.rc_family  = AF_BLUETOOTH;
        socket_address_.rc_channel = (uint8_t) channel_;
    }
#elif _WIN32
	is_valid_ &= StringToAddress(address_, socket_address_.btAddr) == 0;

	if (is_valid_) {
		socket_address_.addressFamily  = AF_BTH;
		socket_address_.serviceClassId = GUID_NULL;
		socket_address_.port           = BT_PORT_ANY;
	}
#endif
}

#ifdef _WIN32
// http://social.msdn.microsoft.com/Forums/en-US/9d47f33c-c848-4728-ae36-f251623daf17/bluetooth-101-wsastringtoaddress
int BluetoothDevice::StringToAddress(IN const string& address_string,
									 OUT BTH_ADDR& address)
{
	BTH_ADDR temp_address;
	ULONG address_data[6];

	if (address_string.empty()) {
		return -1;
	}

	if (sscanf_s(address_string.c_str(),
		"%02x:%02x:%02x:%02x:%02x:%02x",
		&address_data[0],
		&address_data[1],
		&address_data[2],
		&address_data[3],
		&address_data[4],
		&address_data[5]) != 6)
	{
		return -1;
	}

	for (int i = 0; i < 6; ++i) {
		// Extract data from the first 8 lower bits.
		temp_address = (BTH_ADDR) (address_data[i] & 0xFF);

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
	return is_valid_;
}

string BluetoothDevice::ToString() const
{
	ostringstream result;

	result << "BluetoothDevice"
		<< " \"" << name_ << "\""
		<< " @ " << address_
		<< " on channel " << channel_;

	return result.str();
}

BluetoothDevice::SocketAddress BluetoothDevice::GetSocketAddress() const
{
    return socket_address_;
}
