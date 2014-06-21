/*      CommonSerial (abstract class)
 *
 *  This abstract class provides functionality shared by most serial interfaces
 *  such as Bluetooth or USB. Specific methods have to be implemented by sub-
 *  classed serial interfaces.
 *
 *  See ISerial for example usage.
 */

#ifndef DJ_GLOVE_PC_COMMON_COMMON_SERIAL_H_
#define DJ_GLOVE_PC_COMMON_COMMON_SERIAL_H_

#include "ISerial.h"

class CommonSerial : public ISerial {
public:
    // ISerial
    virtual bool HasAvailable() const;
    virtual std::string ReadNextAvailable();
    virtual std::string Read();
    virtual int Write(const std::string& output);
    virtual int WriteLine(const std::string& output);

    // ISerial, implement in subclass
    virtual void WaitUntilAvailable() = 0;
    virtual int Available() const = 0;
    virtual std::string Read(const int length) = 0;
    virtual int Write(const std::string& output, const int length) = 0;
};

#endif // DJ_GLOVE_PC_COMMON_COMMON_SERIAL_H_

