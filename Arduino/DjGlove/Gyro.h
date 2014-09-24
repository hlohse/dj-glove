#ifndef DJ_GLOVE_ARDUINO_DJ_GLOVE_GYRO_H_
#define DJ_GLOVE_ARDUINO_DJ_GLOVE_GYRO_H_

class Gyro {
public:
  typedef struct Readout {
    int x;
    int y;
    int z;
  } Readout;
  
  Gyro(const byte address)
  : m_address(address),
    m_readout()
  {
    memset(&m_readout, 0, sizeof(m_readout));
    initialize();
  }
  
  Readout read()
  {
    if (hasAvailable()) {
      readValue(m_readout.x, register_x_low);
      readValue(m_readout.y, register_y_low);
      readValue(m_readout.z, register_z_low);
    }
    
    return m_readout;
  }

private:
  static const byte settings         = B00011111; // x, y, z; normal mode; 100Hz @ 25 cut-off
  static const byte status_available = B00001000; // x, y, z available
  static const byte register_ctrl_0  = 0x20;
  static const byte register_status  = 0x27;
  static const byte register_x_low   = 0x28;
  static const byte register_y_low   = 0x30;
  static const byte register_z_low   = 0x32;
  
  byte    m_address;
  Readout m_readout;
  
  void initialize()
  {
    Wire.beginTransmission(m_address);
    Wire.write(register_ctrl_0);
    Wire.write(settings);
    Wire.endTransmission();
  }
  
  bool hasAvailable() const 
  {
    return readRegisterByte(register_status) & status_available;
  }
  
  void readValue(int& destination, const byte register_low)
  {
    destination  = readRegisterByte(register_low);
    destination |= readRegisterByte(register_low + 1) << 8;
  }
  
  byte readRegisterByte(const byte reg) const
  {
    Wire.beginTransmission(m_address);
    Wire.write(reg);
    Wire.endTransmission();
    Wire.requestFrom(m_address, (byte) 1);
    while (!Wire.available());
    return Wire.read();
  }
};

#endif // DJ_GLOVE_ARDUINO_DJ_GLOVE_GYRO_H_

