/*-----------------------------------------------------------------------------
	Axis.h

	By MultipleMonomials and ChatterComa, thx to Kliment
-----------------------------------------------------------------------------*/

#ifndef AXIS_H
#define AXIS_H

#include "AEONS_Config.h"
#include "Arduino.h"
#include "pin.h"

/*-----------------------------------------------------------------------------
	Controls a movement axis
-----------------------------------------------------------------------------*/
struct Axis
{
public:
	float _current_position;
	float _steps_per_mm;

private:
	bool _endstop_pin_inverting;
	bool _has_endstop;
	bool _current_direction_positive;
	bool _endstop_at_MIN;
	bool endstop_cleared_to_move;

#ifdef DEBUG_ENDSTOPS
	uint8_t _message_counter;
#endif

	DigitalOutputPin _step_pin;
	DigitalOutputPin _enable_pin;
	DigitalOutputPin _direction_pin;
	DigitalInputPin _endstop_pin;

public:
	float _max_feedrate;
	float _homing_feedrate;

	Axis(Pin step_pin, Pin direction_pin, Pin enable_pin, Pin endstop_pin, bool direction_pin_inverting, bool endstop_pin_inverting, bool endstop_at_MIN, float steps_per_mm, float max_feedrate, float homing_feedrate, unsigned int step_delay);

	void set_positive_direction(bool positive_direction);

	void enable();

	void disable();

	void step();

	void update_endstop_clearance();

private:
	bool cleared_to_move();

public:
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
