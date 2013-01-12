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
	s_value =(int) get_value_from_char_array(command, 'S');
	if(s_value == 0)
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

/*------------------------------------------------------------------------------------
M116 [Ssss] [Pppp]-- Wait for extruder and bed to heat up (can specify temps for both)
------------------------------------------------------------------------------------*/

M116::M116(char * command)
{
	//user can optionally set temp values
	s_value =(int) get_value_from_char_array(command, 'S');
	p_value =(int) get_value_from_char_array(command, 'P');


}

void M116::process()
{
	bool extruder_up_to_temp =
	#ifdef HAS_EXTRUDER
		false;
	#else
		true;
	#endif

	//If we don't have a bed we'll just say it's already hot
	bool bed_up_to_temp =
	#ifdef HAS_BED
		false;
	#else
		true;
	#endif

	do
	{
		//not too cold
		#ifdef HAS_EXTRUDER
			extruder_up_to_temp =
					(Printer::instance().Extruder.getTemperature() >= (Printer::instance().Extruder.getTarget() - TEMPDELTA)) &&
					(Printer::instance().Extruder.getTemperature() <= (Printer::instance().Extruder.getTarget() + TEMPDELTA));
			Serial.print("ok T:");
			Serial.print(Printer::instance().Extruder.getTemperature());
		#endif

		//calling Extruder.getTemperature automatically manages the temperature
		#ifdef HAS_BED
			bed_up_to_temp =
					(Printer::instance().Bed.getTemperature() >= (Printer::instance().Bed.getTarget() - TEMPDELTA)) &&
					(Printer::instance().Bed.getTemperature() <= (Printer::instance().Bed.getTarget() + TEMPDELTA));
			Serial.print(" B:");
			Serial.println(Printer::instance().Bed.getTemperature());
		#endif

		delay(2000);
	}
	while(!(extruder_up_to_temp && bed_up_to_temp));

}

/*-----------------------------------------------------------------------------
M140 <SXXX>--Set bed temperature to given temp
-----------------------------------------------------------------------------*/

M140::M140(char * command)
{
	if((s_value =(int) get_value_from_char_array(command, 'S')) == 0)
	{
		Serial.println("No argument provided for M140, setting temp to 0...");
	}

}

// Will set temp to 0 if argument not found, I consider this good.

void M140::process()
{
	#ifdef DEBUG_GCODE_PROCESSING
		Serial.print("Setting bed temperature to: ");
		Serial.print(s_value);
		Serial.println(" Degrees C");
	#endif
	#ifdef HAS_BED
		Printer::instance().Bed.setTemperature(s_value);
	#endif

}
