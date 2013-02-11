/*----------------------------------__-----------------------------------------
	Printer.cpp
	 
	By MultipleMonomials and ChatterComa, thx to Kliment
-----------------------------------------------------------------------------*/
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
		Fan(FAN_PIN, false),
	#endif

	// Axis(Pin step_pin, Pin direction_pin, Pin enable_pin, float steps_per_mm, float _max_feedrate, float homing_feedrate, bool axis_relative_mode);
	x_axis(X_STEP_PIN, X_DIR_PIN, X_ENABLE_PIN, X_STEPS_PER_MM, XY_AXES_MAX_FEEDRATE, XY_AXES_HOMING_FEEDRATE),
	y_axis(Y_STEP_PIN, Y_DIR_PIN, Y_ENABLE_PIN, Y_STEPS_PER_MM, XY_AXES_MAX_FEEDRATE, XY_AXES_HOMING_FEEDRATE),
	z_axis(Z_STEP_PIN, Z_DIR_PIN, Z_ENABLE_PIN, Z_STEPS_PER_MM, Z_AXIS_MAX_FEEDRATE, ZE_AXES_HOMING_FEEDRATE),
	e_axis(E_STEP_PIN, E_DIR_PIN, E_ENABLE_PIN, E_STEPS_PER_MM, E_AXIS_MAX_FEEDRATE, ZE_AXES_HOMING_FEEDRATE)
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

	// Select one of these only to define how the extruder temp is read.
	extruder_heater_device = THERMISTOR;
	//extruder_heater_device = AD595;
	//MAX6675 CURRENTLY NOT SUPPORTED

	// Select one of these only to define how the bed temp is read.
	bed_heater_device = THERMISTOR;
	//bed_heater_device = AD595;

	//Set the custom commands to run when an M40 is recieved.
	//MAKE SURE IT ENDS WITH \0!!!
	/* EXAMPLE:*/m40_commands = (char*) "M140 S5 \n G1 Y200 F2000 \0";

	relative_mode = false;

	last_feedrate = 100;
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


