#include "Serial.h"
#include <sstream>
using namespace std;

bool Serial::HasAvailable() const
{
    return Available() > 0;
}

string Serial::Read()
{
    return Read(Available());
}

int Serial::Write(const string& output)
{
    return Write(output, output.length());
}

int Serial::WriteLine(const string& output)
{
    ostringstream line;

    line << output << endl;

    return Write(line.str());
}

