/*----------------------------------__-----------------------------------------
	assert.h
	 
	By MultipleMonomials and ChatterComa, thx to Kliment
-----------------------------------------------------------------------------*/

#ifndef ASSERT_H
#define ASSERT_H

#include "Arduino.h"

// Fatal error handler.
#define ASSERT(__value__) 											\
if (!__value__)														\
{																	\
	Serial.print("Fatal Error!\n Check the line in firmware: ");						\
	Serial.println(__LINE__);										\
	while(true){}													\
}


#endif