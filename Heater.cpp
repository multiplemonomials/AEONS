/*-----------------------------------------------------------------------------
	Heater.cpp
	 
	By MultipleMonomials and ChatterComa, thx to Kliment
-----------------------------------------------------------------------------*/

#include "Heater.h"


/*-----------------------------------------------------------------------------
	Ctor.
-----------------------------------------------------------------------------*/
Heater::Heater(Thermometer deviceType, Pin sensorPin, Pin powerPin, const TempTableElement* givenTable)
{
	_device_type = deviceType;
	_target = 0;
	_current = 0;
	_temptable = givenTable;
	_sensor_pin = sensorPin;
	_power_pin = powerPin;
	pinMode(_sensor_pin, INPUT);
	pinMode(_power_pin, OUTPUT);
}

/*-----------------------------------------------------------------------------
	Return current temperature.
-----------------------------------------------------------------------------*/
TempInDegrees Heater::getTemperature()
{
	update_local_temperature();
	return _current;
}

/*-----------------------------------------------------------------------------
-----------------------------------------------------------------------------*/
void Heater::setTemperature(TempInDegrees target)
{
	_target = target;
	manage_temperature();
}

/*-----------------------------------------------------------------------------
	Read thermistor and update class variables.
-----------------------------------------------------------------------------*/
void Heater::update_local_temperature()
{
	if (_device_type == THERMISTOR)
	{
		RawAdcValue current_raw = 1023 - analogRead(_sensor_pin);
		for (int i=2; i<=_temptable[0].rawAdcValue; i++)
		{
			// Skip until we find a raw value greater than current_raw.
			if (_temptable[i].rawAdcValue > current_raw)
			{
				// Found one.  Interpolate between this and previous table entry.
				RawAdcValue    fractionalRaw = current_raw - _temptable[i-1].rawAdcValue;
				TempInDegrees  stepTemp = _temptable[i].tempInDegrees - _temptable[i-1].tempInDegrees;
				RawAdcValue    stepRaw = _temptable[i].rawAdcValue - _temptable[i-1].rawAdcValue;
				_current  = _temptable[i-1].tempInDegrees + fractionalRaw * stepTemp / stepRaw;

				break;
			}
		}
	}
	
	if (_device_type == AD595)
	{
		RawAdcValue current_raw = analogRead(_sensor_pin);
		_current = current_raw * 500 / 1024;
	}
}

/*-----------------------------------------------------------------------------
-----------------------------------------------------------------------------*/
void Heater::manage_temperature()
{
	update_local_temperature();
	
	//check if we're at a lower temperature than the target
	if(_current + TEMPDELTA < _target)
		digitalWrite(_power_pin, HIGH);
		
	//check if we're at a higher temperature than the target	
	if(_current - TEMPDELTA > _target)
		digitalWrite(_power_pin, LOW);
		
	//else we must be within the allowed delta of the target

}
