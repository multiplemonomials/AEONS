#include "Movement.h"
#include "Assert.h"

/*-----------------------------------------------------------------------------
	Ctor
-----------------------------------------------------------------------------*/
Movement::Movement(float x_target, float y_target, float z_target, float e_target, float feedrate) :
#ifdef DEBUG_MOVEMENT
	_start_millis(millis()),
#endif

	_x_target(x_target),
	_y_target(y_target),
	_z_target(z_target),
	_e_target(e_target),
	_feedrate((int)feedrate!=0 ? feedrate : Printer::instance().last_feedrate),

	_total_x_steps(0),
	_total_y_steps(0),
	_total_z_steps(0),
	_total_e_steps(0),

	_proposed_x_feedrate(0),
	_proposed_y_feedrate(0),
	_proposed_z_feedrate(0),
	_proposed_e_feedrate(0),
	_x_movement_factor(0),
	_y_movement_factor(0),
	_z_movement_factor(0),
	_e_movement_factor(0),

	_x_steps_counter(0),
	_y_steps_counter(0),
	_z_steps_counter(0),
	_e_steps_counter(0),

	_loops_to_do(0),

	_move_distance_in_mm(0),
	_feedrate_mm_per_millisecond(0),
	_move_time_in_ms(0),
	_time_in_ms_per_loop(0),

	_calculation_time_millisconds(0),

	_delayer()

{
	// Further calculation deferred until calculate_values().
}

//for unit tests
Movement::Movement() :
#ifdef DEBUG_MOVEMENT
	_start_millis(millis()),
#endif

	_x_target(0),
	_y_target(0),
	_z_target(0),
	_e_target(0),
	_feedrate(0),

	_total_x_steps(0),
	_total_y_steps(0),
	_total_z_steps(0),
	_total_e_steps(0),

	_proposed_x_feedrate(0),
	_proposed_y_feedrate(0),
	_proposed_z_feedrate(0),
	_proposed_e_feedrate(0),
	_x_movement_factor(0),
	_y_movement_factor(0),
	_z_movement_factor(0),
	_e_movement_factor(0),

	_x_steps_counter(0),
	_y_steps_counter(0),
	_z_steps_counter(0),
	_e_steps_counter(0),

	_loops_to_do(0),

	_move_distance_in_mm(0),
	_feedrate_mm_per_millisecond(0),
	_move_time_in_ms(0),
	_time_in_ms_per_loop(0),
	_global_steps_per_mm(0),

	_calculation_time_millisconds(0),

	_delayer()

{

}

/*-----------------------------------------------------------------------------
	calculate intervals and feedrates to prepare for move
	works in RELATIVE MODE
-----------------------------------------------------------------------------*/
void Movement::calculate_values()
{
	// Truncate the move coordinates to be within the printer's printable area.
	restrict_to_printable_area();

	update_current_position();

	// If some moves are negative, set the axis to move in a negative direction,
	// then convert to positive value.
	absolute_valueize_direction();

	mm_to_steps();

	enable_axes();

	limit_feedrates_to_maximums();

	calculate_movement_ratios();

	update_endstop_clearances();

	calculate_delays();

	print_debug_values();
}

//-------------------------------------------------------------------------------
// Over-distance protection
// AKA software endstops
//-------------------------------------------------------------------------------
void Movement::restrict_to_printable_area()
{
	#ifdef MAX_SOFTWARE_ENDSTOPS
	//maximum
	if(_x_target + Printer::instance().x_axis._current_position > X_MAX_LENGTH)
	{
		_x_target =(float) X_MAX_LENGTH - Printer::instance().x_axis._current_position;
	}

	if(_y_target + Printer::instance().y_axis._current_position > Y_MAX_LENGTH)
	{
		_y_target =(float) Y_MAX_LENGTH - Printer::instance().x_axis._current_position;
	}

	if(_z_target + Printer::instance().z_axis._current_position > Z_MAX_LENGTH)
	{
		_z_target =(float) Z_MAX_LENGTH - Printer::instance().z_axis._current_position;
	}
	#endif

	//minimum
	#ifdef MIN_SOFTWARE_ENDSTOPS
	if(_x_target + Printer::instance().x_axis._current_position < 0)
	{
		_x_target = 0.0 - Printer::instance().x_axis._current_position;
	}

	if(_y_target + Printer::instance().y_axis._current_position < 0)
	{
		_y_target = 0.0 - Printer::instance().x_axis._current_position;
	}

	if(_z_target + Printer::instance().z_axis._current_position < 0)
	{
		_z_target = 0.0 - Printer::instance().z_axis._current_position;
	}
	#endif
}

//-------------------------------------------------------------------------------
//  Tell the axes where they are now
//  It has to go here because it needs to see the targets before they are absolute valued
//-------------------------------------------------------------------------------
void Movement::update_current_position()
{
	Printer::instance().x_axis.setCurrentPosition(Printer::instance().x_axis.getCurrentPosition() + _x_target);
	Printer::instance().y_axis.setCurrentPosition(Printer::instance().y_axis.getCurrentPosition() + _y_target);
	Printer::instance().z_axis.setCurrentPosition(Printer::instance().z_axis.getCurrentPosition() + _z_target);
}
//-------------------------------------------------------------------------------
//  if some moves are negative, set the axis to move in a negative direction,
// then convert to positive value.
//-------------------------------------------------------------------------------
void Movement::absolute_valueize_direction()
{
	Printer::instance().x_axis.set_positive_direction((_x_target >= 0));
	_x_target = abs(_x_target);

	Printer::instance().y_axis.set_positive_direction((_y_target >= 0));
	_y_target = abs(_y_target);

	Printer::instance().z_axis.set_positive_direction((_z_target >= 0));
	_z_target = abs(_z_target);

	Printer::instance().e_axis.set_positive_direction((_e_target >= 0));
	_e_target = abs(_e_target);
}

//-------------------------------------------------------------------------------
// Convert from mm to steps for each axis.
//-------------------------------------------------------------------------------
void Movement::mm_to_steps()
{
	_total_x_steps = _x_target * Printer::instance().x_axis._steps_per_mm;
	_total_y_steps = _y_target * Printer::instance().y_axis._steps_per_mm;
	_total_z_steps = _z_target * Printer::instance().z_axis._steps_per_mm;
	_total_e_steps = _e_target * Printer::instance().e_axis._steps_per_mm;
}

//-------------------------------------------------------------------------------
// Enable movement for axes that need to move.
//-------------------------------------------------------------------------------
void Movement::enable_axes()
{
	if(_total_x_steps != 0)
	{
		Printer::instance().x_axis.enable();
	}

	if(_total_y_steps != 0)
	{
		Printer::instance().y_axis.enable();
	}

	if(_total_z_steps != 0)
	{
		Printer::instance().z_axis.enable();
	}

	if(_total_e_steps != 0)
	{
		Printer::instance().e_axis.enable();
	}
}

//-------------------------------------------------------------------------------
// Make sure feedrates are within limits
//-------------------------------------------------------------------------------
void Movement::limit_feedrates_to_maximums()
{
	if(_feedrate > Printer::instance().x_axis._max_feedrate && !(_x_target == 0))
	{
	_proposed_x_feedrate = Printer::instance().x_axis._max_feedrate;
	}
	else
	{
	_proposed_x_feedrate = _feedrate;
	}

	if(_feedrate > Printer::instance().y_axis._max_feedrate && !(_y_target == 0))
	{
	_proposed_y_feedrate = Printer::instance().y_axis._max_feedrate;
	}
	else
	{
	_proposed_y_feedrate = _feedrate;
	}

	if(_feedrate > Printer::instance().z_axis._max_feedrate && !(_z_target == 0))
	{
	_proposed_z_feedrate = Printer::instance().z_axis._max_feedrate;
	}
	else
	{
	_proposed_z_feedrate = _feedrate;
	}

	if(_feedrate > Printer::instance().e_axis._max_feedrate && !(_e_target == 0))
	{
	_proposed_e_feedrate = Printer::instance().e_axis._max_feedrate;
	}
	else
	{
	_proposed_e_feedrate = _feedrate;
	}


	_feedrate = min(min(_proposed_x_feedrate, _proposed_y_feedrate), min(_proposed_z_feedrate, _proposed_e_feedrate));
}

//-------------------------------------------------------------------------------
// Calculate loops per tick and axis steps per loop
//-------------------------------------------------------------------------------
void Movement::calculate_movement_ratios()
{
	_loops_to_do = LCM(_total_x_steps, _total_y_steps, _total_z_steps, _total_e_steps);

	_x_movement_factor = (_total_x_steps == 0) ? (_loops_to_do + 1) : (_loops_to_do / _total_x_steps);
	_y_movement_factor = (_total_y_steps == 0) ? (_loops_to_do + 1) : (_loops_to_do / _total_y_steps);
	_z_movement_factor = (_total_z_steps == 0) ? (_loops_to_do + 1) : (_loops_to_do / _total_z_steps);
	_e_movement_factor = (_total_e_steps == 0) ? (_loops_to_do + 1) : (_loops_to_do / _total_e_steps);
}

//-------------------------------------------------------------------------------
// Update endstop direction so we don't have to do this while stepping
//-------------------------------------------------------------------------------
void Movement::update_endstop_clearances()
{
	//don't need to do this if we aren't checking endstops during moves
	#ifdef ENDSTOP_CHECK_DURING_MOVE
		Printer::instance().x_axis.update_endstop_clearance();
		Printer::instance().y_axis.update_endstop_clearance();
		Printer::instance().z_axis.update_endstop_clearance();
		Printer::instance().e_axis.update_endstop_clearance();
	#endif
}


//-------------------------------------------------------------------------------
// Calculate delays from feedrate
//-------------------------------------------------------------------------------
void Movement::calculate_delays()
{
	// Calculate Delayer per mm of movement to achieve the stated feed rate.
	// Is E a factor here?
	_move_distance_in_mm 			= sqrt(pow(sqrt(pow(sqrt(pow(_x_target, 2) + pow(_y_target, 2)), 2) + pow(_z_target, 2)), 2) + pow(_e_target, 2));
	_feedrate_mm_per_millisecond 	= _feedrate / 60000.0;
	_move_time_in_ms				= _move_distance_in_mm / _feedrate_mm_per_millisecond;
	_time_in_ms_per_loop			= _move_time_in_ms / _loops_to_do;
	_global_steps_per_mm			= (float)_loops_to_do/(float)_move_distance_in_mm;
	float delay_seconds_per_step 	= ((1.0/_feedrate_mm_per_millisecond)/1000.0)/_global_steps_per_mm;
	_delayer 						= new Delayer(_loops_to_do, _time_in_ms_per_loop, delay_seconds_per_step);
}


//-------------------------------------------------------------------------------
// Debug logging.
//-------------------------------------------------------------------------------
void Movement::print_debug_values()
{
	#ifdef DEBUG_MOVEMENT
		unsigned long calculation_time_millisconds = millis() - _start_millis;

		#define DISPLAY_IT(__val) Serial.print(#__val); Serial.print(": "); Serial.println(__val)
		Serial.println("Calling move function with parameters:");
		DISPLAY_IT(_time_in_ms_per_loop);
		DISPLAY_IT(_loops_to_do);
		DISPLAY_IT(_total_x_steps);
		DISPLAY_IT(_total_y_steps);
		DISPLAY_IT(_total_z_steps);
		DISPLAY_IT(_total_e_steps);
		DISPLAY_IT(_x_movement_factor);
		DISPLAY_IT(_y_movement_factor);
		DISPLAY_IT(_z_movement_factor);
		DISPLAY_IT(_e_movement_factor);
		DISPLAY_IT(_x_target);
		DISPLAY_IT(_y_target);
		DISPLAY_IT(_z_target);
		DISPLAY_IT(_e_target);
		DISPLAY_IT(_move_time_in_ms);
		DISPLAY_IT(_move_distance_in_mm);
		DISPLAY_IT(_global_steps_per_mm);
		DISPLAY_IT(_feedrate);
		DISPLAY_IT(_feedrate_mm_per_millisecond);
		Serial.print("Calculation took: ");
		Serial.println(calculation_time_millisconds);
		#undef DISPLAY_IT
	#endif
}

/*-----------------------------------------------------------------------------
	Call the final function!!
-----------------------------------------------------------------------------*/
void Movement::execute()
{
	step_loop();

	delete _delayer;

	Printer::instance().last_feedrate = _feedrate;

	#ifdef DEBUG_MOVEMENT
	Serial.println("Finished Move!");
	Serial.print("Move took ");
	Serial.print(millis() - _start_millis);
	Serial.println("milliseconds");
	#endif

	#ifdef DISABLE_AXES_AFTER_MOVE
		//-------------------------------------------------------------------------------
		// Disable Axes
		//-------------------------------------------------------------------------------
		Printer::instance().x_axis.disable();
		Printer::instance().y_axis.disable();
		Printer::instance().z_axis.disable();
		Printer::instance().e_axis.disable();
	#endif
}

//-------------------------------------------------------------------------------
// Returns false if the movement is invalid, e.g., X0 Y0 Z0 E0
//-------------------------------------------------------------------------------
bool Movement::is_valid()
{
	return(!(_x_target == 0 && _y_target == 0 && _z_target == 0 && _e_target == 0));
}

/*-----------------------------------------------------------------------------
	Modulo operation with special case for zero-valued dividend.
-----------------------------------------------------------------------------*/
int16_t Movement::modulo(int dividend, int divisor)
{
	return (dividend == 0) ? 0 : dividend % divisor;
}

/*-----------------------------------------------------------------------------
 	 Multiply operation that always returns a non-zero result, even
 	 if zero-valued arguments arte provided.
-----------------------------------------------------------------------------*/
int Movement::multiply_no_0(int value1, int value2)
{
	if(value1 == 0)
	{
		value1 = 1;
	}

	if(value2 == 0)
	{
		value2 = 1;
	}

	return value1 * value2;
}


/*-----------------------------------------------------------------------------
	Greatest Common Divisor.  Returns the largest value that all arguments
	can be divided by leaving no remainder.
-----------------------------------------------------------------------------*/
uint16_t Movement::GCD(unsigned int x_val, unsigned int y_val)
{
	if (x_val == 0 && y_val == 0)
		{
			return 1;
		}
	if (x_val == 0)
	{
		x_val = 1;
	}

	if(y_val == 0)
	{
		y_val = 1;
	}
    for (;;)
    {
        if (x_val == 0)
        {
        	return y_val;
        }

        y_val = y_val % x_val;

        if (y_val == 0)
        {
        	return x_val;
        }

        x_val = x_val % y_val;
    }

    return 1;

}

/*-----------------------------------------------------------------------------
	Least Common Multiple.  The smallest positive integer that is divisible by
	all supplied arguments. Zero arguments don't count.
-----------------------------------------------------------------------------*/
uint16_t Movement::LCM(unsigned int x_val, unsigned int y_val, unsigned int z_val, unsigned int e_val)
{
	// Algorithm check.
	//ASSERT( (multiply_no_0(x_val, y_val) % GCD(x_val, y_val)) == 0);
	//ASSERT( ((z_val * e_val) % GCD(z_val, e_val)) == 0);

	unsigned int LCM_of_x_and_y = (multiply_no_0(x_val, y_val)) / GCD(x_val, y_val); //264/1
	unsigned int LCM_of_z_and_e = (multiply_no_0(z_val, e_val)) / GCD(z_val, e_val); //1/1
	unsigned int final_LCM 		= (multiply_no_0(LCM_of_x_and_y, LCM_of_z_and_e)) / GCD(LCM_of_x_and_y, LCM_of_z_and_e);

#if 0
	Serial.println("LCM: ");
	Serial.println(LCM_of_x_and_y);
	Serial.println(LCM_of_z_and_e);
	Serial.println(final_LCM);
	Serial.println("xy: ");
	Serial.print(multiply_no_0(x_val, y_val));
	Serial.println("GCD: ");
	Serial.print(GCD(x_val, y_val));
	Serial.println("END");
#endif

	return final_LCM;


}


/*-----------------------------------------------------------------------------

-----------------------------------------------------------------------------*/
void Movement::step_loop()

{
	_x_steps_counter = _x_movement_factor;
	_y_steps_counter = _y_movement_factor;
	_z_steps_counter = _z_movement_factor;
	_e_steps_counter = _e_movement_factor;

	for(int tick_counter = _loops_to_do; tick_counter > 0; tick_counter--)
	{
		if(--_x_steps_counter == 0)
		{
			Printer::instance().x_axis.step();
			_x_steps_counter = _x_movement_factor;
		}

		if(--_y_steps_counter == 0)
		{
			Printer::instance().y_axis.step();
			_y_steps_counter = _y_movement_factor;
		}

		if(--_z_steps_counter == 0)
		{
			Printer::instance().z_axis.step();
			_z_steps_counter = _z_movement_factor;
		}

		if(--_e_steps_counter == 0)
		{
			Printer::instance().e_axis.step();
			_e_steps_counter = _e_movement_factor;
		}

		(*_delayer)();
	}

}
