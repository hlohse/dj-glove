/*      ISerial (interface)
 *
 *  This interface defines functionality provided by serial interfaces such
 *  as Bluetooth or USB.
 *
 *  Example, echo back input data twice:
 *
 *      ISerial serial = ... (use implementing subclass)
 *
 *      if (serial.IsReady()) {
 *          serial.WaitUntilAvailable();
 *          serial.Write(serial.Read());
 *
 *          serial.WriteLine(serial.ReadNextAvailable());
 *      }
 *      else {
 *          cout << serial.GetLastError() << endl;
 *      }
 */

#ifndef DJ_GLOVE_PC_COMMON_ISERIAL_H_
#define DJ_GLOVE_PC_COMMON_ISERIAL_H_

#include <string>

class ISerial {
public:
    // Returns true if serial interface is useable
    virtual bool IsReady() const = 0;

    // Returns last error message once serial interface is not ready anymore
    virtual std::string GetLastError() const = 0;

    // Blocks execution until input becomes available.
    virtual void WaitUntilAvailable() = 0;
    
    // Signals if input contains data.
    virtual bool HasAvailable() const = 0;
    
    // Number of input bytes available to be read.
    virtual int Available() const = 0;

    // Blocks execution until input becomes available.
    // Returns all available input bytes.
    virtual std::string ReadNextAvailable() = 0;

    // Returns all available input bytes.
    virtual std::string Read() = 0;

    // Returns up to length available input bytes.
    virtual std::string Read(const int length) = 0;

    // Writes output bytes.
    // Returns number of actually written bytes of output.
    virtual int Write(const std::string& output) = 0;

    // Writes up to length bytes of output.
    // Returns number of actually written bytes of output.
    virtual int Write(const std::string& output, const int length) = 0;

    // Writes output bytes with line break.
    // Returns number of actually written bytes of output.
    virtual int WriteLine(const std::string& output) = 0;
};

#endif // DJ_GLOVE_PC_COMMON_ISERIAL_H_

