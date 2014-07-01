#include "Bluetooth.h"
#include <SoftwareSerial.h>

// D pin connected to Bluetooth module's TXD
static const int rx = 2;

// D pin connected to Bluetooth module's RXD
static const int tx = 3;

// Baud rate for Bluetooth module
static const int baud = 9600;

SoftwareSerial Bluetooth(rx, tx);

void BluetoothSetup()
{
    Bluetooth.begin(baud);
}

