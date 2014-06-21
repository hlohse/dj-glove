#include "BluetoothDevice.h"
#include <sstream>
using namespace std;

BluetoothDevice::BluetoothDevice(const string& name,
                                 const string& address)
:   name_(name),
    address_(address)
{
    const int result = str2ba(address_.c_str(), &socket_address_.rc_bdaddr);

    is_valid_ = result == 0;
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
           << " @ " << address_;

    return result.str();
}

