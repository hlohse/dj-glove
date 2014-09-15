#ifndef DJ_GLOVE_ARDUINO_DJ_GLOVE_ULTRA_SOUND_H_
#define DJ_GLOVE_ARDUINO_DJ_GLOVE_ULTRA_SOUND_H_

#include <Wire.h>

class UltraSound {
public:
  enum unit {
    UNIT_IN = 0x50, // Distance in inches
    UNIT_CM = 0x51, // Distance in centimeters
    UNIT_MS = 0x52  // Distance in ping microseconds
  };
  
  UltraSound(const byte device_id, const unit unit = UNIT_MS)
  : m_device_id(device_id),
    m_unit(unit),
    m_value(0)
  {
  }
  
  void initialize()
  {
    setUnit(m_unit);
    startRead();
  }
  
  void setUnit(const unit unit)
  {
    m_unit = unit;
    Wire.beginTransmission(m_device_id);
    setRegister(REGISTER_COMMAND);
    Wire.write(m_unit);
    Wire.endTransmission();
  }
  
  // Return new value, if available. Previous value otherwise.
  int read()
  {
    Wire.requestFrom(m_device_id, num_read_bytes);
    
    if (Wire.available() >= num_read_bytes) {
      const byte high = Wire.read();
      const byte low  = Wire.read();
      m_value = (((int) high) << 8) | low;
      startRead();
    }
    
    return m_value;
  }

private:
  enum registers {
    REGISTER_COMMAND = 0x00,
    REGISTER_ECHO_1  = 0x02
  };
  
  static const byte num_read_bytes = 2;
  
  byte m_device_id;
  unit m_unit;
  int  m_value;
  
  void setRegister(const registers reg) const
  {
    Wire.write(byte(reg));
  }
  
  void startRead() const
  {
    Wire.beginTransmission(m_device_id);
    setRegister(REGISTER_ECHO_1);
    Wire.endTransmission();
  }
};

#endif // DJ_GLOVE_ARDUINO_DJ_GLOVE_ULTRA_SOUND_H_

