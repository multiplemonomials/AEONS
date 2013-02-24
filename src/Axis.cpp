/*-----------------------------------------------------------------------------
	Axis.cpp

	By MultipleMonomials and ChatterComa, thx to Kliment
-----------------------------------------------------------------------------*/
#include "Axis.h"

Axis::Axis(Pin step_pin, Pin direction_pin, Pin enable_pin, Pin endstop_pin, bool direction_pin_inverting, bool endstop_pin_inverting, bool endstop_at_MIN, float steps_per_mm, float max_feedrate, float homing_feedrate, unsigned int step_delay)
:
	_step_pin(step_pin, step_delay, true),
	_enable_pin(enable_pin, 1, !ENABLE_PINS_INVERTING),
	_direction_pin(direction_pin, 1, !direction_pin_inverting),
	_endstop_pin(endstop_pin)

{
	_homing_feedrate = homing_feedrate;
	_steps_per_mm = steps_per_mm;
	_max_feedrate = max_feedrate;
	_current_position = 0;
	_endstop_pin_inverting = endstop_pin_inverting;
	_endstop_at_MIN = endstop_at_MIN;

	_has_endstop = (endstop_pin > 0);

	_current_direction_positive = true;
}

void Axis::set_positive_direction(bool positive_direction)
{
	//store the current direction for endstop testing
	_current_direction_positive = positive_direction;

	if(positive_direction)
	{
		_direction_pin.setActive();
	}
	else
	{
		_direction_pin.setInactive();
	}
}

void Axis::enable()
{
	_enable_pin.setActive(); //DigitalOutputPin takes care of the inversion for us.
}

void Axis::disable()
{
	_enable_pin.setInactive();
}

bool Axis::check_endstop_active()
{
	if(!_has_endstop)
	{
		return false; //return false always if the pin is set to -1
	}
	else if(_current_direction_positive == _endstop_at_MIN )
	{
		return false; //ignore the endstop if we're moving away from it
	}
	else if(_endstop_pin_inverting)
	{
		return !_endstop_pin.isActive();
	}
	{
		return _endstop_pin.isActive();
	}
}

void Axis::step()
{
	#ifdef ACTUALLY_MOVE
		if(!check_endstop_active())
		{
			_step_pin.pulse();
		}
	#endif
}
