
# relative?
ARDUINO_DIR   = /usr/share/arduino
ARDMK_DIR     = $(realpath ../)

AVR_TOOLS_DIR = /usr

ARDUINO_LIBS = SoftwareSerial Bluetooth
USER_LIB_PATH = $(realpath ../common)
BOARD_TAG = nano328
MONITOR_PORT = /dev/ttyUSB0

include $(ARDMK_DIR)/Arduino.mk

CPPFLAGS += -I$(USER_LIB_PATH)


