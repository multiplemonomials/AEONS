/*----------------------------------__-----------------------------------------
	assert.h
	 
	By MultipleMonomials and ChatterComa, thx to Kliment
-----------------------------------------------------------------------------*/

#ifndef ASSERT_H
#define ASSERT_H

#include "Arduino.h"
#include "AEONS_Config.h"

// Fatal error handler.
#define ASSERT(__bool__) 											\
if (!__bool__)														\
{																	\
	Serial.print("Fatal Error in: ");								\
	Serial.print(__FILE__);											\
	Serial.print(", line: ");										\
	Serial.println(__LINE__);										\
	digitalWrite(PS_ON_PIN, HIGH);									\
	while(true){}													\
}

#define ASSERT2(__bool__, __value__) 								\
if (!(__bool__))														\
{																	\
	Serial.print("Fatal Error in: ");								\
	Serial.print(__FILE__);											\
	Serial.print(", line: ");										\
	Serial.println(__LINE__);										\
	Serial.print("Debug value: ");									\
	Serial.println(__value__);										\
	digitalWrite(PS_ON_PIN, HIGH);									\
	while(true){}													\
}
#endif
