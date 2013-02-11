/*-----------------------------------------------------------------------------
	Axis.h

	By MultipleMonomials and ChatterComa, thx to Kliment
-----------------------------------------------------------------------------*/

#ifndef AXIS_H
#define AXIS_H

#include "AEONS_Typedefs.h"
#include "AEONS_Config.h"
#include "Arduino.h"

/*-----------------------------------------------------------------------------
	Controls a movement axis
-----------------------------------------------------------------------------*/
struct Axis
{
	float _current_position;
	float _steps_per_mm;

	Pin _enable_pin;
	Pin _step_pin;
	Pin _direction_pin;

	float _max_feedrate;
	float _homing_feedrate;

	Axis(Pin step_pin, Pin direction_pin, Pin enable_pin, float steps_per_mm, float _max_feedrate, float homing_feedrate);

	void set_positive_direction(bool positive_direction);

	void enable();

	void disable();

	void step();

	float getCurrentPosition() const
	{
		return _current_position;
	}

	void setCurrentPosition(float currentPosition)
	{
		_current_position = currentPosition;
	}


};

#endif
