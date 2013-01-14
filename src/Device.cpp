/*----------------------------------__-----------------------------------------
	Device.cpp
	 
	By MultipleMonomials and ChatterComa, thx to Kliment
-----------------------------------------------------------------------------*/

#include "Device.h"
#include "Arduino.h"
#include "AEONS_Typedefs.h"

/*-----------------------------------------------------------------------------
-----------------------------------------------------------------------------*/
Device::Device(Pin onPin, bool inverting)
{
	_inverting = inverting;
	_on_pin = onPin;
	pinMode(_on_pin, OUTPUT);
}

/*-----------------------------------------------------------------------------
-----------------------------------------------------------------------------*/
void Device::turn_on()
{
	digitalWrite(_on_pin, _inverting ? LOW : HIGH);
	#ifdef DEBUG_GCODE_PROCESSING
		Serial.print("Setting pin ");
		Serial.print(_on_pin);
		Serial.println(_inverting ? " to LOW" : " to HIGH");
	#endif
}

/*-----------------------------------------------------------------------------
-----------------------------------------------------------------------------*/
void Device::turn_off()
{
	digitalWrite(_on_pin, _inverting ? HIGH : LOW);
}
