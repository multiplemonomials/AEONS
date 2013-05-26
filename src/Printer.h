/*---------------------------------------------------------------------------
	Printer.h
	 
	By MultipleMonomials and ChatterComa
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

	#ifdef HAS_SECOND_EXTRUDER
	Heater Extruder_2;
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
	Axis e_axis_0;
	Axis & e_axis; //warning about this should be OK, it is trying to compress a reference wich is uncompressible
				   //the only way to fix this is to change the compiler command, not possible with the Arduino IDE
	Axis e_axis_1;


	char* m40_commands;

	//holds the input gcode
	char command [MAX_GCODE_LENGTH];

	bool relative_mode;

	float last_feedrate;

	uint32_t max_acceleration;

	
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
