#include "CommonSerial.h"
#include <sstream>
using namespace std;

bool CommonSerial::HasAvailable() const
{
    return Available() > 0;
}

string CommonSerial::ReadNextAvailable()
{
    WaitUntilAvailable();

    return Read();
}

string CommonSerial::Read()
{
    return Read(Available());
}

int CommonSerial::Write(const string& output)
{
    return Write(output, output.length());
}

int CommonSerial::WriteLine(const string& output)
{
    ostringstream line;

    line << output << endl;

    return Write(line.str());
}

