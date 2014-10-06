#ifndef DJ_GLOVE_ARDUINO_DJ_GLOVE_ACCELERATION_H_
#define DJ_GLOVE_ARDUINO_DJ_GLOVE_ACCELERATION_H_

class Acceleration {
public:
  typedef struct {
    int x;
    int y;
    int z;
  } Readout;
  
  Acceleration(const byte address, const byte pin_int)
  : m_address(address),
    m_pin_int(pin_int)
  {
  }
  
  void initialize() const
  {
    pinMode(m_pin_int, INPUT);
    digitalWrite(m_pin_int, HIGH);
    
    // reset (01000000) and wait...
    writeRegister(register_ctrl_reg2, 0x40);
    while (readRegister(register_ctrl_reg2) & 0x40);
    
    //enable 8G range (00000010)
    writeRegister(register_xyz_data_cfg, 0x02);
  
    //disable FIFO-mode (00000000)
    writeRegister(register_f_setup, 0x00); 
   
    //enable data-ready-interrupt
    writeRegister(register_ctrl_reg4, 0x01); 
    writeRegister(register_ctrl_reg4, 0x01);
    writeRegister(register_ctrl_reg5, 0x01);
  
    // activate sensor with DR = 400Hz (00001001)
    writeRegister(register_ctrl_reg1, 0x09);
  }
    
  
  bool hasAvailable() const
  {
    return digitalRead(m_pin_int) == LOW;
  }
  
  Readout read() const
  {
    Readout readout;
    
    Wire.beginTransmission(m_address);
    Wire.write(register_out_x_msb);
    Wire.endTransmission(false);
    Wire.requestFrom(m_address, (byte) 6);

    readAxis(readout.x);
    readAxis(readout.y);
    readAxis(readout.z);
    
    return readout;
  }

private:

  static const byte register_out_x_msb = 0x01;
  static const byte register_ctrl_reg1 = 0x2A;
  static const byte register_ctrl_reg2 = 0x2B;
  static const byte register_ctrl_reg4 = 0x2D;
  static const byte register_ctrl_reg5 = 0x2E;
  static const byte register_xyz_data_cfg = 0x0E; 
  static const byte register_f_setup = 0x09;
  
  byte m_address;
  byte m_pin_int;
  
  void readAxis(int& axis) const
  {
    axis = Wire.read();
    axis <<= 8; 
    axis |= Wire.read();
    axis >>= 2;
  }
  
  void writeRegister(byte registerAddress, byte newValue) const
  {
     Wire.beginTransmission(m_address);
     Wire.write(registerAddress);
     Wire.write(newValue);
     Wire.endTransmission();
  }
  
  byte readRegister(byte registerAddress) const
  {
    Wire.beginTransmission(m_address);
    Wire.write(registerAddress);
    Wire.endTransmission(false);
    Wire.requestFrom(m_address, (byte)1);
    return Wire.read();
  }
  
}; 

#endif // DJ_GLOVE_ARDUINO_DJ_GLOVE_ACCELERATION_H_

