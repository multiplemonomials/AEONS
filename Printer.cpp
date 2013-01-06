/*----------------------------------__-----------------------------------------
	Printer.cpp
	 
	By MultipleMonomials and ChatterComa, thx to Kliment
-----------------------------------------------------------------------------*/
#ifdef PRINTER_CPP

#else 
#define PRINTER_CPP
#include "Device.h"
#include "Heater.h"
#include "Printer.h"


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
	
	_instance = 0;
	
		//// Calibration variables
	// X, Y, Z, E steps per unit
	axis_steps_per_mm[0] = 52.504;
	axis_steps_per_mm[1] = 26.523;
	axis_steps_per_mm[2] = 2267.567;
	axis_steps_per_mm[3] = 760.0; 
	
	// Select one of these only to define how the extruder temp is read.
	Thermometer extruder_heater_device = THERMISTOR;
	//Thermometer extruder_heater_device = AD595;
	//MAX6675 CURRENTLY NOT SUPPORTED

	// Select one of these only to define how the bed temp is read.
	Thermometer bed_heater_device = THERMISTOR;
	//Thermometer bed_heater_device = AD595;
	
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
