#!/bin/bash

TTYUSB=$1   # E.g. /dev/ttyUSB0
BINARY=$2   # E.g. Binaries/dj-glove.hex

avrdude -F -p atmega328p -P $1 -c arduino -b 57600 -U flash:w:$2

