#include "Bluetooth.h"
#include "BluetoothDevice.h"
using namespace std;

Bluetooth::Bluetooth(const BluetoothDevice& device)
{
}

Bluetooth::~Bluetooth()
{
}

bool Bluetooth::HasAvailable() const
{
    return false;
}

int Bluetooth::Available() const
{
    return 0;
}

string Bluetooth::Read()
{
    return "";
}

string Bluetooth::Read(const int length)
{
    return "";
}

int Bluetooth::Write(const string& output)
{
    return 0;
}

int Bluetooth::Write(const string& output, const int length)
{
    return 0;
}

int Bluetooth::WriteLine(const string& output)
{
    return 0;
}

