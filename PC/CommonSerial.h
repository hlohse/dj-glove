/*      CommonSerial (abstract class)
 *
 *  This abstract class provides functionality shared by most serial interfaces
 *  such as Bluetooth or USB. Specific methods have to be implemented by sub-
 *  classed serial interfaces.
 *
 *  See ISerial for example usage.
 */

#ifndef DJ_GLOVE_PC_COMMON_SERIAL_H_
#define DJ_GLOVE_PC_COMMON_SERIAL_H_

#include "ISerial.h"
#include "Export.h"
#include <sstream>

class EXPORT CommonSerial : public ISerial {
public:
    // ISerial
    virtual std::string GetLastError() const;
    virtual bool HasAvailable() const;
    virtual std::string ReadNextAvailable(const int length);
    virtual int WriteLine(const std::string& output);

    // ISerial, implement in subclass
    virtual bool IsReady() const = 0;
    virtual void WaitUntilAvailable(const int length) = 0;
    virtual int Available() const = 0;
    virtual std::string Read(const int length) = 0;
    virtual int Write(const std::string& output) = 0;

protected:
    // Used by subclasses to store last error message
	std::ostringstream last_error_;
};

#endif // DJ_GLOVE_PC_COMMON_SERIAL_H_

