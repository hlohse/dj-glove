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
  
  UltraSound(const byte address, const unit unit)
  : m_address(address),
    m_unit(unit),
    m_value(0),
    m_init_time(0xFFFFFFFF)
  {
  }
  
  void initialize()
  {
    Wire.beginTransmission(m_address);
    Wire.write(register_command);
    Wire.write((byte) m_unit);
    Wire.endTransmission();
    m_init_time = millis();
  }
  
  // Return new value, if available. Previous value otherwise.
  int read()
  {
    if (isSensorInitialized()) {
      requestBytes();
      
      if (hasBytes()) {
        updateValue();
      }
      
      initialize();
    }
    
    return m_value;
  }

private:
  static const byte register_command = 0x00;
  static const byte register_echo_1  = 0x02;
  static const byte num_read_bytes   = 2;
  
  byte          m_address;
  unit          m_unit;
  int           m_value;
  unsigned long m_init_time;
  
  bool isSensorInitialized() const
  {
    const unsigned long passed_ms = millis() - m_init_time;
    return passed_ms >= 70; // Datasheet suggests 65ms
  }
  
  void requestBytes() const
  {
    Wire.beginTransmission(m_address);
    Wire.write(register_echo_1);
    Wire.endTransmission();
  }
  
  bool hasBytes() const
  {
    return Wire.requestFrom(m_address, num_read_bytes) >= num_read_bytes;
  }
  
  void updateValue()
  {
    const byte high = Wire.read();
    const byte low  = Wire.read();
    m_value = (((int) high) << 8) | low;
  }
};

#endif // DJ_GLOVE_ARDUINO_DJ_GLOVE_ULTRA_SOUND_H_

