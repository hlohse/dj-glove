/*
 *  This interfaces defines functionality provided by serial interfaces such as
 *  Bluetooth or USB.
 *
 *  Example, echo back input data:
 *
 *      ISerial serial = ... (use implementing subclass)
 *
 *      if (serial.WaitUntilAvailable()) {
 *          serial.Write(serial.Read());
 *      }
 *      else {
 *          cout << "ERROR: " << serial.GetLastError() << endl;
 *      }
 */

#ifndef DJ_GLOVE_PC_COMMON_ISERIAL_H_
#define DJ_GLOVE_PC_COMMON_ISERIAL_H_

#include <string>

class ISerial {
public:
    // Signals if input contains data.
    virtual bool HasAvailable() const = 0;
    
    // Number of input bytes available to be read.
    virtual int Available() const = 0;

    // Blocks execution until input becomes available.
    // Returns false if an error occured, true otherwise. (See GetLastError.)
    virtual bool WaitUntilAvailable() = 0;

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

    // Returns error message of the last occured error.
    virtual std::string GetLastError() const = 0;
};

#endif // DJ_GLOVE_PC_COMMON_ISERIAL_H_

