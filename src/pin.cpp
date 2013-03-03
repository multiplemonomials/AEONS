/*-----------------------------------------------------------------------------
	Pin.cpp

	By MultipleMonomials and ChatterComa, thx to Kliment
-----------------------------------------------------------------------------*/
#include "pin.h"

DigitalInputPin::DigitalInputPin(Pin pin_number)
{
	_pin_number = pin_number;

	if(_pin_number > 0)
	{
		pinMode(_pin_number, INPUT);
	}
}

bool DigitalInputPin::isActive()
{
	if(_pin_number > 0)
	{
		return digitalRead(_pin_number) == HIGH ? true : false;
	}
	else
	{
		return false;
	}
}

/*-----------------------------------------------------------------------------

-----------------------------------------------------------------------------*/
DigitalOutputPin::DigitalOutputPin(Pin pin_number, uint16_t pulse_duration,  bool active_high)
{
	_pin_number = pin_number; //the pin to work with
	_pulse_duration = pulse_duration; //if pulse() is called, how long in microseconds to wait?
	_active_high = active_high;

	pinMode(_pin_number, OUTPUT);
	digitalWrite(_pin_number, active_high ? LOW : HIGH);

	//delayMicroseconds will die horribly if called with more than 16383
	ASSERT2(_pulse_duration < 16383, _pulse_duration);
}

void DigitalOutputPin::setActive()
{
	digitalWrite(_pin_number, _active_high ? HIGH : LOW);
}

void DigitalOutputPin::setInactive()
{
	digitalWrite(_pin_number, _active_high ? LOW : HIGH);
}

void DigitalOutputPin::pulse()
{
	digitalWrite(_pin_number, _active_high ? HIGH : LOW); //turn on
	//millis();
	delayMicroseconds(_pulse_duration);
	digitalWrite(_pin_number, _active_high ? LOW : HIGH); //turn off
}

/*-----------------------------------------------------------------------------
Analog Input Pin
-----------------------------------------------------------------------------*/
AnalogInputPin::AnalogInputPin(Pin pin_number, bool active_high)
{
	_pin_number = pin_number;
	_active_high = active_high;

	pinMode(_pin_number, INPUT);
}

int AnalogInputPin::get_value()
{
	return digitalRead(_pin_number);
}

/*-----------------------------------------------------------------------------

-----------------------------------------------------------------------------*/
AnalogOutputPin::AnalogOutputPin(Pin pin_number)
{
	_pin_number = pin_number;

	pinMode(_pin_number, OUTPUT);
}

void AnalogOutputPin::set_value(uint8_t value)
{
	analogWrite(_pin_number, value);
}

