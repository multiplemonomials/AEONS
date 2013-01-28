#include "Movement.h"
#include "Assert.h"

int modulo(int dividend, int divisor)
{
	if (dividend == 0)
			return 0;
	else
		return dividend%divisor;
}

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

-----------------------------------------------------------------------------*/
unsigned int LCM(unsigned int x_val, unsigned int y_val, unsigned int z_val, unsigned int e_val)
{
	// Algorithm check.
	ASSERT( ((x_val * y_val) % GCD(x_val, y_val)) == 0);
	ASSERT( ((z_val * e_val) % GCD(z_val, e_val)) == 0);

	unsigned int LCM_of_x_and_y = (multiply_no_0(x_val, y_val)) / GCD(x_val, y_val);
	unsigned int LCM_of_z_and_e = (multiply_no_0(z_val, e_val)) /	GCD(z_val, e_val);
	unsigned int final_LCM 		= (multiply_no_0(LCM_of_x_and_y, LCM_of_z_and_e)) / GCD(LCM_of_x_and_y, LCM_of_z_and_e);
	return final_LCM;

}


/*-----------------------------------------------------------------------------

-----------------------------------------------------------------------------*/
int GCD(int x_val, int y_val)
{
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

-----------------------------------------------------------------------------*/
void step_loop
(
	unsigned int time_in_ms_per_loop,
	unsigned int loop_count,
	StepCount x_steps_per_tick,
	StepCount y_steps_per_tick,
	StepCount z_steps_per_tick,
	StepCount e_steps_per_tick
)
{
	StepCount x_steps_counter = x_steps_per_tick;
	StepCount y_steps_counter = y_steps_per_tick;
	StepCount z_steps_counter = z_steps_per_tick;
	StepCount e_steps_counter = e_steps_per_tick;

	for(int tick_counter = loop_count; tick_counter > 0; tick_counter--)
	{
		if(x_steps_counter-- != 0)
		{
			Printer::instance().x_axis.step();
			x_steps_counter = x_steps_per_tick;
		}

		if(y_steps_counter-- != 0)
		{
			Printer::instance().y_axis.step();
			y_steps_counter = y_steps_per_tick;
		}

		if(z_steps_counter-- != 0)
		{
			Printer::instance().z_axis.step();
			z_steps_counter = z_steps_per_tick;
		}

		if(e_steps_counter-- != 0)
		{
			Printer::instance().e_axis.step();
			e_steps_counter = e_steps_per_tick;
		}

		delay(time_in_ms_per_loop);
	}
}

/*-----------------------------------------------------------------------------

-----------------------------------------------------------------------------*/
void move(float x_target, float y_target, float z_target, float e_target, float feedrate)
{
	//-------------------------------------------------------------------------------
	// Enable Axes
	//-------------------------------------------------------------------------------
	Printer::instance().x_axis.enable();
	Printer::instance().y_axis.enable();
	Printer::instance().z_axis.enable();
	Printer::instance().e_axis.enable();

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
	// Calculate loops per tick and axis steps per loop
	//-------------------------------------------------------------------------------
	unsigned int loops_to_do = LCM(total_x_steps, total_y_steps, total_z_steps, total_e_steps);

	StepCount x_interval = loops_to_do / total_x_steps;
	StepCount y_interval = loops_to_do / total_y_steps;
	StepCount z_interval = loops_to_do / total_z_steps;
	StepCount e_interval = loops_to_do / total_e_steps;

	//-------------------------------------------------------------------------------
	// Calculate delays from feedrate
	//-------------------------------------------------------------------------------

	// Calculate delay per mm of movement to achieve the stated feed rate.
	long move_distance_in_mm            = x_target + y_target + z_target + e_target;
	float feedrate_mm_per_millisecond 	= feedrate / (60.0 * 1000.0);
	// float delay_millisecond_per_mm 		= move_distance_in_mm + feedrate/ feedrate_mm_per_millisecond;
	float move_time_in_ms				= move_distance_in_mm / feedrate_mm_per_millisecond;
	float time_in_ms_per_loop			= move_time_in_ms / loops_to_do;

//	unsigned long x_delay_per_step = delay_millisecond_per_mm / Printer::instance().x_axis._steps_per_mm;
//	unsigned long y_delay_per_step = delay_millisecond_per_mm / Printer::instance().y_axis._steps_per_mm;
//	unsigned long z_delay_per_step= delay_millisecond_per_mm / Printer::instance().z_axis._steps_per_mm;
//	unsigned long e_delay_per_step = delay_millisecond_per_mm / Printer::instance().e_axis._steps_per_mm;
//	unsigned long delay_per_loop = (total_x_steps * x_delay_per_step) + (total_y_steps)


//	void step_loop(unsigned int loops_to_do, unsigned int ticks_per_loop, unsigned int x_steps_per_tick, unsigned int y_steps_per_tick, unsigned int z_steps_per_tick,
//			unsigned int e_steps_per_tick, unsigned long delay_per_x_step, unsigned long delay_per_y_step, unsigned long delay_per_z_step, unsigned long delay_per_e_step)

	//-------------------------------------------------------------------------------
	// Debug logging.
	//-------------------------------------------------------------------------------

	#ifdef DEBUG_MOVEMENT
		#define DISPLAY_IT(__val) Serial.print(#__val); Serial.println(__val)
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
		#undef DISPLAY_IT
	#endif

	//-------------------------------------------------------------------------------
	// Call the final function!!
	//-------------------------------------------------------------------------------

	step_loop(time_in_ms_per_loop, loops_to_do, x_interval, y_interval, z_interval, e_interval);

	Printer::instance().last_feedrate = feedrate;

#ifdef DEBUG_MOVEMENT
	Serial.println("Finished Move!");
#endif
}
