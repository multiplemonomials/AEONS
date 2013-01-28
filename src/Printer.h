/*----------------------------------__-----------------------------------------
	Printer.h
	 
	By MultipleMonomials and ChatterComa, thx to Kliment
-----------------------------------------------------------------------------*/

#ifndef PRINTER_H
#define PRINTER_H
#include "Device.h"
#include "Heater.h"
#include "Axis.h"


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
	
	Thermometer extruder_heater_device;

	Thermometer bed_heater_device;
	
	Axis x_axis;
	Axis y_axis;
	Axis z_axis;
	Axis e_axis;

	char* m40_commands;

	//holds the input gcode
	char command [MAX_GCODE_LENGTH];

	bool relative_mode;

	float last_feedrate;

	
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
