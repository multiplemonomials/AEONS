/*----------------------------------__-----------------------------------------
	assert.h
	 
	By MultipleMonomials and ChatterComa, thx to Kliment
-----------------------------------------------------------------------------*/

#ifndef ASSERT_H
#define ASSERT_H

#include "Arduino.h"

// Fatal error handler.
#define ASSERT(__bool__) 											\
if (!__bool__)														\
{																	\
	Serial.print("Fatal Error!\n Check the line in firmware: ");	\
	Serial.println(__LINE__);										\
	while(true){}													\
}

#define ASSERT2(__bool__, __value__) 								\
if (!__bool__)														\
{																	\
	Serial.print("Fatal Error!\n Check the line in firmware: ");	\
	Serial.println(__LINE__);										\
	Serial.print("Debug value: ");									\
	Serial.println(__value__);										\
	while(true){}													\
}
#endif