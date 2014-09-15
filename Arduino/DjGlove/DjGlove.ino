#include <Bluetooth.h>
#include <SoftwareSerial.h>
#include "Pins.h"
#include "Push.h"
#include "Poti.h"
#include "Flex.h"
#include "Led.h"
#include "DjGlove.h"
#include "Data.h"
 
DjGlove   glove; // Only instantiation!
Bluetooth bluetooth(Pins::bt_rx, Pins::bt_tx);

void setup()  
{
}

void loop()
{  
}

