/*----------------------------------__-----------------------------------------
	Printer.cpp
	 
	By MultipleMonomials and ChatterComa, thx to Kliment
-----------------------------------------------------------------------------*/
#ifdef PRINTER_CPP

#else 
#define PRINTER_CPP

#include "Printer.h"

/*-----------------------------------------------------------------------------
	Instance variable of this singleton object.
-----------------------------------------------------------------------------*/

Printer * Printer::_instance = 0;


/*-----------------------------------------------------------------------------
	Ctor.
-----------------------------------------------------------------------------*/

Printer::Printer()
:
	#ifdef HAS_POWER_SUPPLY
		Power_Supply(PS_ON_PIN, true),
	#endif
	#ifdef HAS_EXTRUDER
		Extruder(extruder_heater_device, TEMP_0_PIN, HEATER_0_PIN, TEMPTABLE),
	#endif
	#ifdef HAS_BED
		Bed(extruder_heater_device, TEMP_1_PIN, HEATER_1_PIN, BEDTEMPTABLE),
	#endif
	#ifdef HAS_FAN
		Fan(FAN_PIN, false)
	#endif
	
{
	#ifdef HAS_POWER_SUPPLY
		Power_Supply.turn_on();
	#endif
	
	#ifdef HAS_EXTRUDER
		Extruder.setTemperature(DEFAULT_EXTRUDER_TEMP);
	#endif
	
	#ifdef HAS_BED
		Bed.setTemperature(DEFAULT_BED_TEMP);
	#endif

	#ifdef HAS_FAN
		Fan.turn_on();
	#endif
	

	
		//// Calibration variables
	// X, Y, Z, E steps per unit
	axis_steps_per_mm[0] = 52.504;
	axis_steps_per_mm[1] = 26.523;
	axis_steps_per_mm[2] = 2267.567;
	axis_steps_per_mm[3] = 760.0; 
	
	// Select one of these only to define how the extruder temp is read.
	extruder_heater_device = THERMISTOR;
	//extruder_heater_device = AD595;
	//MAX6675 CURRENTLY NOT SUPPORTED

	// Select one of these only to define how the bed temp is read.
	bed_heater_device = THERMISTOR;
	//bed_heater_device = AD595;
	
	//// MOVEMENT SETTINGS
	max_feedrate[0] = 18000;
	max_feedrate[1] = 18000;
	max_feedrate[2] = 100;
	max_feedrate[3] = 18000;
	
	homing_feedrate[0] = 1500;
	homing_feedrate[1] = 1500;
	homing_feedrate[2] = 100;
	
	axis_relative_modes[0] = false;
	axis_relative_modes[1] = false;
	axis_relative_modes[2] = false;
	axis_relative_modes[3] = false;

	//Set the custom commands to run when an M40 is recieved.
	//MAKE SURE IT ENDS WITH \0!!!
	/* EXAMPLE:*/m40_commands = "M140 S5 \n G1 Y200 F2000 \0";
}


/*-----------------------------------------------------------------------------
	Get a reference to the single, system-wide instance of Printer.
-----------------------------------------------------------------------------*/
Printer & Printer::instance()
{
	if (_instance == 0)
	{
		_instance = new Printer();
	}
	
	return *_instance;
}



//Printer::instance().fan.turn_on();


#endif
