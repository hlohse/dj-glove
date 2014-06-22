/*      ISerial (interface)
 *
 *  This interface defines functionality provided by serial interfaces such
 *  as Bluetooth or USB.
 *
 *  Example, echo back any size input data twice:
 *
 *      ISerial serial = ... (use implementing subclass)
 *      const int data_size = 8;
 *
 *      if (serial.IsReady()) {
 *          serial.Write(serial.ReadNextAvailable(data_size));
 *
 *          serial.WaitUntilAvailable(data_size);
 *          serial.WriteLine(serial.Read(data_size));
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
    virtual ~ISerial() {}

    // Returns true if serial interface is useable, false otherwise.
    virtual bool IsReady() const = 0;

    // Returns last error message once serial interface is not ready anymore.
    virtual std::string GetLastError() const = 0;

    // Blocks execution until length input bytes become available.
    virtual void WaitUntilAvailable(const int length) = 0;
    
    // Signals if input contains data.
    virtual bool HasAvailable() = 0;
    
    // Number of input bytes available to be read.
    virtual int Available() = 0;

    // Blocks execution until length input bytes become available.
    // Returns length available input bytes.
    virtual std::string ReadNextAvailable(const int length) = 0;

    // Returns up to length available input bytes.
    virtual std::string Read(const int length) = 0;

    // Writes output bytes.
    // Returns number of actually written bytes of output.
    virtual int Write(const std::string& output) = 0;

    // Writes output bytes with line break.
    // Returns number of actually written bytes of output.
    virtual int WriteLine(const std::string& output) = 0;
};

#endif // DJ_GLOVE_PC_COMMON_ISERIAL_H_

