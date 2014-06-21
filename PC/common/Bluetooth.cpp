#include "Bluetooth.h"
#include "BluetoothDevice.h"
using namespace std;

Bluetooth::Bluetooth(const BluetoothDevice& device)
{
}

Bluetooth::~Bluetooth()
{
}

int Bluetooth::Available() const
{
    return 0;
}

void Bluetooth::WaitUntilAvailable()
{
}

string Bluetooth::Read(const int length)
{
    return "";
}

int Bluetooth::Write(const string& output, const int length)
{
    return 0;
}

