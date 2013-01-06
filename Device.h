/*-----------------------------------------------------------------------------
	Device.h
	 
	By MultipleMonomials and ChatterComa, thx to Kliment
-----------------------------------------------------------------------------*/

#ifndef DEVICE_H
#define DEVICE_H

#include "AEONS_Typedefs.h"

/*-----------------------------------------------------------------------------
	Manages simple I/O device that turns on and off.
-----------------------------------------------------------------------------*/
struct Device
{
	Pin 		_on_pin;
	bool 	_inverting;
	
	//constructor
	Device(Pin onPin, bool inverting);
	
	void turn_on();
	
	void turn_off();
};

#endif