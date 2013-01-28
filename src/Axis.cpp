/*-----------------------------------------------------------------------------
	Axis.cpp

	By MultipleMonomials and ChatterComa, thx to Kliment
-----------------------------------------------------------------------------*/
#include "Axis.h"

Axis::Axis(Pin step_pin, Pin direction_pin, Pin enable_pin, float steps_per_mm, float max_feedrate, float homing_feedrate, bool axis_relative_mode)
{
	_step_pin = step_pin;
	_enable_pin = enable_pin;
	_direction_pin = direction_pin;

	_axis_relative_mode = axis_relative_mode;
	_homing_feedrate = homing_feedrate;
	_steps_per_mm = steps_per_mm;
	_max_feedrate = max_feedrate;
	_current_position = 0;
}

void Axis::set_positive_direction(bool positive_direction)
{

}

void Axis::enable()
{
	digitalWrite(_enable_pin, ENABLE_PINS_INVERTING);

}

void Axis::disable()
{
	digitalWrite(_enable_pin, !ENABLE_PINS_INVERTING);
}

void Axis::step()
{

}
