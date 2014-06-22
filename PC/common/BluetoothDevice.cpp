#include "BluetoothDevice.h"
#include <sstream>
#include <stdint.h>
using namespace std;

BluetoothDevice::BluetoothDevice(const string& name,
                                 const string& address,
                                 const int channel)
:   name_(name),
    address_(address),
    channel_(channel)
{
    const int result = str2ba(address_.c_str(), &socket_address_.rc_bdaddr);

    is_valid_ = result == 0 && channel >= 1 && channel <= 12;

    if (is_valid_) {
        socket_address_.rc_family  = AF_BLUETOOTH;
        socket_address_.rc_channel = (uint8_t) channel_;
    }
}

BluetoothDevice::~BluetoothDevice()
{
}

struct sockaddr_rc BluetoothDevice::GetSocketAddress() const
{
    return socket_address_;
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

