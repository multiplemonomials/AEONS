#include "Movement.h"
#include "Assert.h"

/*-----------------------------------------------------------------------------
	Modulo operation with special case for zero-valued dividend.
-----------------------------------------------------------------------------*/
int modulo(int dividend, int divisor)
{
	return (dividend == 0) ? 0 : dividend % divisor;
}

/*-----------------------------------------------------------------------------
 	 Multiply operation that always returns a non-zero result, even
 	 if zero-valued arguments arte provided.
-----------------------------------------------------------------------------*/
int multiply_no_0(int value1, int value2)
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
unsigned int GCD(unsigned int x_val, unsigned int y_val)
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
unsigned int LCM(unsigned int x_val, unsigned int y_val, unsigned int z_val, unsigned int e_val)
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
void step_loop
(
	delay_base* 	delayer,
	unsigned int 	loop_count,
	StepCount 		x_steps_per_tick,
	StepCount 		y_steps_per_tick,
	StepCount 		z_steps_per_tick,
	StepCount 		e_steps_per_tick
)
{
	StepCount x_steps_counter = x_steps_per_tick;
	StepCount y_steps_counter = y_steps_per_tick;
	StepCount z_steps_counter = z_steps_per_tick;
	StepCount e_steps_counter = e_steps_per_tick;

	for(int tick_counter = loop_count; tick_counter > 0; tick_counter--)
	{
		if(--x_steps_counter == 0)
		{
			Printer::instance().x_axis.step();
			x_steps_counter = x_steps_per_tick;
		}

		if(--y_steps_counter == 0)
		{
			Printer::instance().y_axis.step();
			y_steps_counter = y_steps_per_tick;
		}

		if(--z_steps_counter == 0)
		{
			Printer::instance().z_axis.step();
			z_steps_counter = z_steps_per_tick;
		}

		if(--e_steps_counter == 0)
		{
			Printer::instance().e_axis.step();
			e_steps_counter = e_steps_per_tick;
		}

		(*delayer)();
	}

	delete delayer;
}

/*-----------------------------------------------------------------------------

-----------------------------------------------------------------------------*/
void move(float x_target, float y_target, float z_target, float e_target, float feedrate)
{
	//-------------------------------------------------------------------------------
	// Don't move at all if we didn't get any arguments
	//-------------------------------------------------------------------------------
	if(	x_target == 0 &&
		y_target == 0 &&
		z_target == 0 &&
		e_target == 0)
	{
		return;
	}

	//-------------------------------------------------------------------------------
	//	Debugging for measuring algorithm time
	//-------------------------------------------------------------------------------
	#ifdef DEBUG_MOVEMENT
		unsigned long start_millis = millis();
	#endif

	//-------------------------------------------------------------------------------
	// Over-distance protection
	// AKA software endstops
	//-------------------------------------------------------------------------------

	#ifdef MAX_SOFTRARE_ENDSTOPS
		//maximum
		if(x_target + Printer::instance().x_axis._current_position > X_MAX_LENGTH)
		{
			x_target =(float) X_MAX_LENGTH - Printer::instance().x_axis._current_position;
		}

		if(y_target + Printer::instance().y_axis._current_position > Y_MAX_LENGTH)
		{
			y_target =(float) Y_MAX_LENGTH - Printer::instance().x_axis._current_position;
		}

		if(z_target + Printer::instance().z_axis._current_position > Z_MAX_LENGTH)
		{
			z_target =(float) Z_MAX_LENGTH - Printer::instance().z_axis._current_position;
		}
	#endif

		//minimum
	#ifdef MIN_SOFTWARE_ENDSTOPS
		if(x_target + Printer::instance().x_axis._current_position < 0)
		{
			x_target = 0.0 - Printer::instance().x_axis._current_position;
		}

		if(y_target + Printer::instance().y_axis._current_position < 0)
		{
			y_target = 0.0 - Printer::instance().x_axis._current_position;
		}

		if(z_target + Printer::instance().z_axis._current_position < 0)
		{
			z_target = 0.0 - Printer::instance().z_axis._current_position;
		}
	#endif
	//-------------------------------------------------------------------------------
	// if some moves are negative, set the axis to move in a negative direction,
	// then convert to positive value.
	//-------------------------------------------------------------------------------
	Printer::instance().x_axis.set_positive_direction((x_target >= 0));
	x_target = abs(x_target);

	Printer::instance().y_axis.set_positive_direction((y_target >= 0));
	y_target = abs(y_target);

	Printer::instance().z_axis.set_positive_direction((z_target >= 0));
	z_target = abs(z_target);

	Printer::instance().e_axis.set_positive_direction((e_target >= 0));
	e_target = abs(e_target);

	//-------------------------------------------------------------------------------
	// Convert from mm to steps for each axis.
	//-------------------------------------------------------------------------------
	StepCount total_x_steps = x_target * Printer::instance().x_axis._steps_per_mm;
	StepCount total_y_steps = y_target * Printer::instance().y_axis._steps_per_mm;
	StepCount total_z_steps = z_target * Printer::instance().z_axis._steps_per_mm;
	StepCount total_e_steps = e_target * Printer::instance().e_axis._steps_per_mm;

	//-------------------------------------------------------------------------------
	// Enable Axes
	//-------------------------------------------------------------------------------
	if(total_x_steps != 0)
	{
		Printer::instance().x_axis.enable();
	}

	if(total_y_steps != 0)
	{
		Printer::instance().y_axis.enable();
	}

	if(total_z_steps != 0)
	{
		Printer::instance().z_axis.enable();
	}

	if(total_e_steps != 0)
	{
		Printer::instance().e_axis.enable();
	}

	//-------------------------------------------------------------------------------
	// Make sure feedrates are within limits
	//-------------------------------------------------------------------------------
	{
		int proposed_x_feedrate;
		int proposed_y_feedrate;
		int proposed_z_feedrate;
		int proposed_e_feedrate;

		if(feedrate > Printer::instance().x_axis._max_feedrate && !(x_target == 0))
		{
			proposed_x_feedrate = Printer::instance().x_axis._max_feedrate;
		}
		else
		{
			proposed_x_feedrate = feedrate;
		}

		if(feedrate > Printer::instance().y_axis._max_feedrate && !(y_target == 0))
		{
			proposed_y_feedrate = Printer::instance().y_axis._max_feedrate;
		}
		else
		{
			proposed_y_feedrate = feedrate;
		}

		if(feedrate > Printer::instance().z_axis._max_feedrate && !(z_target == 0))
		{
			proposed_z_feedrate = Printer::instance().z_axis._max_feedrate;
		}
		else
		{
			proposed_z_feedrate = feedrate;
		}

		if(feedrate > Printer::instance().e_axis._max_feedrate && !(e_target == 0))
		{
			proposed_e_feedrate = Printer::instance().e_axis._max_feedrate;
		}
		else
		{
			proposed_e_feedrate = feedrate;
		}


		feedrate = min(min(proposed_x_feedrate, proposed_y_feedrate), min(proposed_z_feedrate, proposed_e_feedrate));
	}


	//-------------------------------------------------------------------------------
	// Calculate loops per tick and axis steps per loop
	//-------------------------------------------------------------------------------
	unsigned int loops_to_do = LCM(total_x_steps, total_y_steps, total_z_steps, total_e_steps);

	StepCount x_interval;
	StepCount y_interval;
	StepCount z_interval;
	StepCount e_interval;

	if(total_x_steps == 0)
	{
		x_interval = loops_to_do + 1;
	}
	else
	{
		x_interval = loops_to_do / total_x_steps;
	}

	if(total_y_steps == 0)
	{
		y_interval = loops_to_do + 1;
	}
	else
	{
		y_interval = loops_to_do / total_y_steps;
	}

	if(total_z_steps == 0)
	{
		z_interval = loops_to_do + 1;
	}
	else
	{
		z_interval = loops_to_do / total_z_steps;
	}

	if(total_e_steps == 0)
	{
		e_interval = loops_to_do + 1;
	}
	else
	{
		e_interval = loops_to_do / total_e_steps;
	}

	//-------------------------------------------------------------------------------
	// Calculate delays from feedrate
	//-------------------------------------------------------------------------------

	// Calculate delay per mm of movement to achieve the stated feed rate.
	long move_distance_in_mm            = x_target + y_target + z_target + e_target;
	float feedrate_mm_per_millisecond 	= feedrate / (60.0 * 1000.0);
	float move_time_in_ms				= move_distance_in_mm / feedrate_mm_per_millisecond;
	float time_in_ms_per_loop			= move_time_in_ms / loops_to_do;

//	void step_loop(unsigned int loops_to_do, unsigned int ticks_per_loop, unsigned int x_steps_per_tick, unsigned int y_steps_per_tick, unsigned int z_steps_per_tick,
//			unsigned int e_steps_per_tick, unsigned long delay_per_x_step, unsigned long delay_per_y_step, unsigned long delay_per_z_step, unsigned long delay_per_e_step)

	//-------------------------------------------------------------------------------
	// Debug logging.
	//-------------------------------------------------------------------------------

	#ifdef DEBUG_MOVEMENT
		unsigned long calculation_time_millisconds = millis() - start_millis;

		#define DISPLAY_IT(__val) Serial.print(#__val); Serial.print(": "); Serial.println(__val)
		Serial.println("Calling move function with parameters:");
		DISPLAY_IT(time_in_ms_per_loop);
		DISPLAY_IT(loops_to_do);
		DISPLAY_IT(total_x_steps);
		DISPLAY_IT(total_y_steps);
		DISPLAY_IT(total_z_steps);
		DISPLAY_IT(total_e_steps);
		DISPLAY_IT(x_interval);
		DISPLAY_IT(y_interval);
		DISPLAY_IT(z_interval);
		DISPLAY_IT(e_interval);
		DISPLAY_IT(move_time_in_ms);
		DISPLAY_IT(move_distance_in_mm);
		Serial.print("Calculation took: ");
		Serial.println(calculation_time_millisconds);
		#undef DISPLAY_IT
	#endif

	//-------------------------------------------------------------------------------
	// Call the final function!!
	//-------------------------------------------------------------------------------

	step_loop(delay_base::delay_factory(time_in_ms_per_loop), loops_to_do, x_interval, y_interval, z_interval, e_interval);

	Printer::instance().last_feedrate = feedrate;

	#ifdef DEBUG_MOVEMENT
		Serial.println("Finished Move!");
		Serial.print("Move took ");
		Serial.print(millis() - start_millis);
		Serial.println("milliseconds");
	#endif

	//-------------------------------------------------------------------------------
	// Disable Axes
	//-------------------------------------------------------------------------------
	Printer::instance().x_axis.disable();
	Printer::instance().y_axis.disable();
	Printer::instance().z_axis.disable();
	Printer::instance().e_axis.disable();
}
