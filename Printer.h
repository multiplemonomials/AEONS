/*----------------------------------__-----------------------------------------
	Printer.h
	 
	By MultipleMonomials and ChatterComa, thx to Kliment
-----------------------------------------------------------------------------*/

#ifdef PRINTER_H

#else
#define PRINTER_H
#include "Device.h"
#include "Heater.h"

/*-----------------------------------------------------------------------------
	Singleton object containing all the printer state.
-----------------------------------------------------------------------------*/
class Printer
{

public:
	#ifdef HAS_POWER_SUPPLY
	Device Power_Supply;
	#endif

	#ifdef HAS_EXTRUDER
	Heater Extruder;
	#endif

	#ifdef HAS_BED
	Heater Bed;
	#endif

	#ifdef HAS_FAN
	Device Fan;
	#endif	
	
	// Pointer to singleton instance.
	static Printer *	_instance;
	
	//// Calibration variables
	// X, Y, Z, E steps per unit
	float axis_steps_per_mm[4]; 
	
	Thermometer extruder_heater_device;

	Thermometer bed_heater_device;
	
	//// MOVEMENT SETTINGS
	float max_feedrate[4];
	float homing_feedrate[3];
	bool axis_relative_modes[4];
	
	
private:


	// Ctor.  Private in this singleton object.
	Printer();
	
public:

	// Get a reference to the single, system-wide instance of Printer.
	static Printer & instance();
};

#endif