#ifndef DJ_GLOVE_PC_COMMON_BLUETOOTH_H_
#define DJ_GLOVE_PC_COMMON_BLUETOOTH_H_

#include "Serial.h"

class BluetoothDevice;

class Bluetooth : public Serial {
public:
    Bluetooth(const BluetoothDevice& device);
    ~Bluetooth();

    // Serial
    virtual int Available() const;
    virtual void WaitUntilAvailable();
    virtual std::string Read(const int length);
    virtual int Write(const std::string& output, const int length);
};

#endif // DJ_GLOVE_PC_COMMON_BLUETOOTH_H_

