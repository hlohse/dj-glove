#include "CommonSerial.h"
using namespace std;

string CommonSerial::GetLastError() const
{
    return last_error_.str();
}

bool CommonSerial::HasAvailable()
{
    return Available() > 0;
}

string CommonSerial::ReadNextAvailable(const int length)
{
    WaitUntilAvailable(length);

    return Read(length);
}

int CommonSerial::WriteLine(const string& output)
{
    ostringstream line;

    line << output << endl;

    return Write(line.str());
}

