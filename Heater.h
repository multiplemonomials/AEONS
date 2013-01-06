/*-----------------------------------------------------------------------------
	Heater.h
	 
	By MultipleMonomials and ChatterComa, thx to Kliment
-----------------------------------------------------------------------------*/

#ifdef HEATER_H

#else
#define HEATER_H

#include "AEONS_Typedefs.h"
#include "Thermometer.h"
#include "AEONS_Config.h"
#include "Arduino.h"

/*-----------------------------------------------------------------------------
	Implementation of smething that has an active heating device,
	typically an extruder, heated build plate, etc.
	
	Has services for setting, getting, and managing the temperature.
-----------------------------------------------------------------------------*/
class Heater
{
	Thermometer  				_device_type;
	TempInDegrees 				_target;
	TempInDegrees 				_current;
	const TempTableElement*   	_temptable;
	Pin                			_sensor_pin; 
	Pin                			_power_pin;

public:
	
	// Ctor.
	Heater(Thermometer deviceType, Pin sensorPin, Pin powerPin, const TempTableElement* givenTable);

	// Return current temperature.
	TempInDegrees getTemperature();
	
	void setTemperature(TempInDegrees target);
	
	// Read thermistor and update class variables.
	void update_local_temperature();
	
	void manage_temperature();
};

#endif