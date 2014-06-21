#ifndef DJ_GLOVE_PC_COMMON_BLUETOOTH_H_
#define DJ_GLOVE_PC_COMMON_BLUETOOTH_H_

#include "ISerial.h"

class BluetoothDevice;

class Bluetooth : public ISerial {
public:
    Bluetooth(const BluetoothDevice& device);
    ~Bluetooth();

    // ISerial
    virtual bool HasAvailable() const;
    virtual int Available() const;
    virtual bool WaitUntilAvailable();
    virtual std::string Read();
    virtual std::string Read(const int length);
    virtual int Write(const std::string& output);
    virtual int Write(const std::string& output, const int length);
    virtual int WriteLine(const std::string& output);
    virtual std::string GetLastError() const;
};

#endif // DJ_GLOVE_PC_COMMON_BLUETOOTH_H_

