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
    pinMode(m_pin_int, INPUT);
    digitalWrite(m_pin_int, HIGH);
  }
  
  bool hasAvailable()
  {
    return digitalRead(m_pin_int) == LOW;
  }
  
  Readout read()
  {
    Readout readout;
    
    Wire.beginTransmission(m_address);
    Wire.write(register_out_x_msb);
    Wire.endTransmission();
    Wire.requestFrom(m_address, (byte) 6);

    readAxis(readout.x);
    readAxis(readout.y);
    readAxis(readout.z);
    
    return readout;
  }

private:
  static const byte register_out_x_msb = 0x01;

  byte m_address;
  byte m_pin_int;
  
  void readAxis(int& axis)
  {
    axis = Wire.read();
    axis <<= 8; 
    axis |= Wire.read();
    axis >>= 2;
  }
}; 

#endif // DJ_GLOVE_ARDUINO_DJ_GLOVE_ACCELERATION_H_

