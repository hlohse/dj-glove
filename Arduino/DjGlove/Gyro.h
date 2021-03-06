#ifndef DJ_GLOVE_ARDUINO_DJ_GLOVE_GYRO_H_
#define DJ_GLOVE_ARDUINO_DJ_GLOVE_GYRO_H_

class Gyro {
public:
  typedef struct Readout {
    int x;
    int y;
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
    
    Wire.beginTransmission(m_address);
    Wire.write(register_ctrl_4);
    Wire.write(B00110000); // 2000dps
    Wire.endTransmission();
  }
  
  Readout read()
  {
    if (hasAvailable()) {
      setRegister(register_readout);
      
      if (hasBytes(4)) {
        readValue(m_readout.x);
        readValue(m_readout.y);
      }
    }
    
    return m_readout;
  }

private:
  static const byte  settings         = B01111011; // x, y; normal mode; 200Hz @ 70 cut-off
  static const byte  status_available = B00000011; // x, y available
  static const byte  register_ctrl_1  = 0x20;
  static const byte  register_ctrl_4  = 0x23;
  static const byte  register_status  = 0x27;
  static const byte  register_readout = B10101000; // Start at 0x28 (x low), auto increment
   
  byte    m_address;
  Readout m_readout;
  
  bool hasAvailable() const 
  {
    setRegister(register_status);
    
    if (hasBytes(1)) {
      return Wire.read() & status_available;
    }
    
    return false;
  }
  
  void readValue(int& destination) const
  {
    destination  = Wire.read();
    destination |= Wire.read() << 8;
  }
  
  void setRegister(const byte reg) const
  {
    Wire.beginTransmission(m_address);
    Wire.write(reg);
    Wire.endTransmission();
  }
  
  bool hasBytes(const int num_bytes) const
  {
    return Wire.requestFrom(m_address, (byte) num_bytes) >= num_bytes;
  }
};

#endif // DJ_GLOVE_ARDUINO_DJ_GLOVE_GYRO_H_

