/*----------------------------------__-----------------------------------------
	gcodes.cpp
	Code file fr all of the gcode objects
	By MultipleMonomials and ChatterComa, thx to Kliment
-----------------------------------------------------------------------------*/

#include "gcodes.h"
#include "AEONS_Config.h"
#include "Arduino.h"
#include "Printer.h"
#include "AEONS.h"

/*-----------------------------------------------------------------------------
M80 Turn On Power Supply
-----------------------------------------------------------------------------*/	

M80::M80(char * command)
{
	// TBD
}

void M80::process()
{
	#ifdef HAS_POWER_SUPPLY
		Printer::instance().Power_Supply.turn_on();
	#endif
}


/*-----------------------------------------------------------------------------
M104 (S230) Set extruder temperature to given temp
-----------------------------------------------------------------------------*/

M104::M104(char * command)
{
	if((s_value =(int) get_value_from_char_array(command, 'S')) = 0)
	{
		Serial.println("No argument provided for M104, setting temp to 0...");
	}
	
}

// Will set temp to 0 if argument not found, I consider this good.

void M104::process()
{
	#ifdef HAS_EXTRUDER
		Printer::instance().Extruder.setTemperature(s_value);
	#endif
}

/*-----------------------------------------------------------------------------
M105 Turn On Power Supply
-----------------------------------------------------------------------------*/	

M105::M105(char * command)
{
	// TBD
}

void M105::process()
{
	#ifdef HAS_EXTRUDER
		Serial.print("ok T:");
		Serial.print(Printer::instance().Extruder.getTemperature()); 
	#endif
	#ifdef HAS_BED
		Serial.print(" B:");
		Serial.println(Printer::instance().Bed.getTemperature());
	#else
		Serial.print('\n');
	#endif
}
