/*
 *  This abstract class defines functionality provided by serial interfaces such
 *  as Bluetooth or USB. Specific methods have to be implemented by subclasses.
 *
 *  Example, echo back input data:
 *
 *      Serial serial = ... (use implementing subclass)
 *
 *      serial.WaitUntilAvailable();
 *      serial.Write(serial.Read());
 */

#ifndef DJ_GLOVE_PC_COMMON_ISERIAL_H_
#define DJ_GLOVE_PC_COMMON_ISERIAL_H_

#include <string>

class Serial {
public:
    // Signals if input contains data.
    bool HasAvailable() const;
    
    // Number of input bytes available to be read.
    virtual int Available() const = 0;

    // Blocks execution until input becomes available.
    virtual void WaitUntilAvailable() = 0;

    // Returns all available input bytes.
    std::string Read();

    // Returns up to length available input bytes.
    virtual std::string Read(const int length) = 0;

    // Writes output bytes.
    // Returns number of actually written bytes of output.
    int Write(const std::string& output);

    // Writes up to length bytes of output.
    // Returns number of actually written bytes of output.
    virtual int Write(const std::string& output, const int length) = 0;

    // Writes output bytes with line break.
    // Returns number of actually written bytes of output.
    int WriteLine(const std::string& output);
};

#endif // DJ_GLOVE_PC_COMMON_ISERIAL_H_

