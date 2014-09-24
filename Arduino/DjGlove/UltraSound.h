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
  
  UltraSound(const byte device_id, const unit unit)
  : m_device_id(device_id),
    m_unit(unit),
    m_value(0),
    m_init_time(0xFFFFFFFF)
  {
    initializeSensor();
  }
  
  // Return new value, if available. Previous value otherwise.
  int read()
  {
    if (isSensorInitialized()) {
      requestBytes();
      
      if (hasBytes()) {
        updateValue();
      }
      
      initializeSensor();
    }
    
    return m_value;
  }

private:
  enum registers {
    REGISTER_COMMAND = 0x00,
    REGISTER_ECHO_1  = 0x02
  };
  
  static const byte num_read_bytes = 2;
  
  byte          m_device_id;
  unit          m_unit;
  int           m_value;
  unsigned long m_init_time;
  
  void initializeSensor()
  {
    Wire.beginTransmission(m_device_id);
    Wire.write((byte) m_device_id);
    Wire.write((byte) m_unit);
    Wire.endTransmission();
    m_init_time = millis();
  }
  
  bool isSensorInitialized() const
  {
    const unsigned long passed_ms = millis() - m_init_time;
    return passed_ms >= 70; // Datasheet suggests 65ms
  }
  
  void requestBytes() const
  {
    Wire.beginTransmission(m_device_id);
    Wire.write((byte) REGISTER_ECHO_1);
    Wire.endTransmission();
    Wire.requestFrom(m_device_id, num_read_bytes);
  }
  
  bool hasBytes() const
  {
    return Wire.available() >= num_read_bytes;
  }
  
  void updateValue()
  {
    const byte high = Wire.read();
    const byte low  = Wire.read();
    m_value = (((int) high) << 8) | low;
  }
};

#endif // DJ_GLOVE_ARDUINO_DJ_GLOVE_ULTRA_SOUND_H_

