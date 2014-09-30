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
  }
  
  void initialize() const
  {
    Wire.beginTransmission(m_address);
    Wire.write(register_ctrl_1);
    Wire.write(settings);
    Wire.endTransmission();
  }
  
  Readout read()
  {
    if (hasAvailable()) {
      Readout readout;
      
      if (readValue(readout.x, register_x_low)
      &&  readValue(readout.y, register_y_low)
      &&  readValue(readout.z, register_z_low))
      {
        m_readout = readout;
      }
    }
    
    return m_readout;
  }

private:
  static const byte settings         = B00011111; // x, y, z; normal mode; 100Hz @ 25 cut-off
  static const byte status_available = B00001000; // x, y, z available
  static const byte register_ctrl_1  = 0x20;
  static const byte register_status  = 0x27;
  static const byte register_x_low   = 0x28;
  static const byte register_y_low   = 0x2A;
  static const byte register_z_low   = 0x2C;
  
  byte    m_address;
  Readout m_readout;
  
  bool hasAvailable() const 
  {
    setRegister(register_status);
    
    if (hasRegisterByte()) {
      return Wire.read() & status_available;
    }
    
    return false;
  }
  
  bool readValue(int& destination, const byte register_low) const
  {
    setRegister(register_low);
    if (!hasRegisterByte()) {
      return false;
    }
    
    destination = Wire.read();
    
    setRegister(register_low + 1);
    if (!hasRegisterByte()) {
      return false;
    }
    
    destination |= Wire.read() << 8;
    return true;
  }
  
  void setRegister(const byte reg) const
  {
    Wire.beginTransmission(m_address);
    Wire.write(reg);
    Wire.endTransmission();
  }
  
  bool hasRegisterByte() const
  {
    return Wire.requestFrom(m_address, (byte) 1) >= 1;
  }
};

#endif // DJ_GLOVE_ARDUINO_DJ_GLOVE_GYRO_H_

