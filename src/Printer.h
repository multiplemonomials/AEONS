/*----------------------------------__-----------------------------------------
	Printer.h
	 
	By MultipleMonomials and ChatterComa, thx to Kliment
-----------------------------------------------------------------------------*/

#ifdef PRINTER_H
#error "Printer redefined"
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

	//// Calibration variables
	// X, Y, Z, E steps per unit
	float axis_steps_per_mm[4]; 
	
	Thermometer extruder_heater_device;

	Thermometer bed_heater_device;
	
	//// MOVEMENT SETTINGS
	float max_feedrate[4];
	float homing_feedrate[3];
	bool axis_relative_modes[4];
	
	char* m40_commands;

	//holds the input gcode
	char command [MAX_GCODE_LENGTH];


	
private:

	// Pointer to singleton instance.
	static Printer * _instance;


	// Ctor.  Private in this singleton object.
	Printer();
	
public:

	// Get a reference to the single, system-wide instance of Printer.
	static Printer & instance();
};

#endif



//Printer *	_instance;
