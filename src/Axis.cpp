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
	_endstop_pin(endstop_pin, endstop_pin_inverting)

{
	_homing_feedrate = homing_feedrate;
	_steps_per_mm = steps_per_mm;
	_max_feedrate = max_feedrate;
	_current_position = 0;
	_endstop_pin_inverting = endstop_pin_inverting;
	_endstop_at_MIN = endstop_at_MIN;

	_has_endstop = (endstop_pin > 0);

	_current_direction_positive = true;

	endstop_cleared_to_move = false;

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

void Axis::update_endstop_clearance()
{
	if(!_has_endstop)
	{
	//return true always if the pin is set to -1
		endstop_cleared_to_move = true;
	}
	else if(_current_direction_positive == _endstop_at_MIN )
	{
		endstop_cleared_to_move = true; //ignore the endstop if we're moving away from it
	}

	else
	{
		endstop_cleared_to_move = false;
	}
}

bool Axis::cleared_to_move()
{
	#ifdef ENDSTOP_CHECK_DURING_MOVE

	if(!endstop_cleared_to_move)
	{
		// If we're here, the endstop exists and we're moving toward it.
		// Cleared to move if we aren't at the end.
		return _endstop_pin_inverting?_endstop_pin.isActive():!_endstop_pin.isActive();
	}

	#endif

	return true;
}

void Axis::step()
{
	#ifdef ACTUALLY_MOVE
		if(cleared_to_move())
		{
			_step_pin.pulse();
			return;
		}
		return;
	#endif
}

void Axis::home()
{
	//set direction according to endstop location
	set_positive_direction(!_endstop_at_MIN);

	// Calculate Delayer per mm of movement to achieve the stated feed rate.
	float step_distance_in_mm           = 1/_steps_per_mm; //move distance == 1 step
	float feedrate_us_per_mm 	= (60.0 * 1000.0 * 1000.0) / _homing_feedrate;
	uint32_t step_time_in_us		= step_distance_in_mm * feedrate_us_per_mm;

	//keep going until we hit an endstop;
	while(_endstop_pin_inverting?_endstop_pin.isActive():!_endstop_pin.isActive())
	{
		step();
		delayMicroseconds(step_time_in_us);
	}

	// Current axis position is the 0 (home) position.
	_current_position = 0;
}
