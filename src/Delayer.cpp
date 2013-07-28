/*-----------------------------------------------------------------------------
	Delayer.cpp

	Uses functors (aka operator ()) to create an object that will Delayer the proper
	amount of time when the <name>() funtion is called.

	By MultipleMonomials and ChatterComa, thx to Kliment
-----------------------------------------------------------------------------*/

#include "Delayer.h"

#include "Assert.h"
#include "Printer.h"
#include "AEONS_Config.h"

/*-----------------------------------------------------------------------------

-----------------------------------------------------------------------------*/
Delayer * Delayer::factory(float min_delay_in_ms, uint16_t total_delayer_calls)
{
	// Validate arguments.
	ASSERT(total_delayer_calls > 1);
	ASSERT(min_delay_in_ms > 0.0);

	// min_delay is smallest possible delay between steps.  Convert to uS.
	DelayTimeInMicroseconds min_delay = min_delay_in_ms * 1000;


	//obtain needed value from Printer
	uint32_t max_acceleration = Printer::instance().max_acceleration;


	//make sure max_acceleration is > 1 mm/s/s
	//or the formula will crash
	ASSERT(max_acceleration > 1);


	//convert from mm/s/s to s/mm/s
	//this is where it gets weird, folks
	float max_accel_s_mm_s = 1.0-(1.0/max_acceleration);

	// This will record length of array; starts at 1
	uint16_t required_array_length_start_end = 1;

	// Initial delay/step.
	const DelayTimeInMicroseconds start_delay = min_delay * ACCEL_START_SPEED_MULTIPLIER;

	DelayTimeInMicroseconds current_delay = 0;

	DelayTimeInMicroseconds previous_delay = start_delay;

	{ // Limit scope of |counter|.

		int counter = 1;

		while(true)
		{
			// See how many subsequent steps use this same delay.

			current_delay = max(min_delay, previous_delay - (previous_delay * max_accel_s_mm_s));

			//is it the same as the last one?
			if(current_delay != previous_delay)
			{
				//order one more spot, plus one spot reserved for the downslope
				++required_array_length_start_end;

				// Update last unique delay.
				previous_delay = current_delay;
			}

			// See if we've finished the acceleration period.
			if(current_delay == min_delay)
			{
				// Done with this calculation.
				break;
			}

			++counter;
		}
	}

	//ORDER UP!
	// Length of array = length of acceleration portion + length of deceleration portion + 1 (for constant-speed middle).
	DelayRun * delay_map = new DelayRun[(required_array_length_start_end * 2) + 1];

	//Is our stuff here?
	ASSERT(delay_map != 0);

	//set the first one
	delay_map[0]._run_delay = min_delay * ACCEL_START_SPEED_MULTIPLIER;
	delay_map[0]._runs_left = 1;

	DelayTimeInMicroseconds delay_buffer;
	int last_unique_value = 0;

	#ifdef ACCELERATION_KILL
		//set everything to min_delay
		delay_map[0]._run_delay = min_delay;
		delay_map[0]._runs_left = total_delayer_calls;
		return new Delayer(delay_map, total_delayer_calls);
	#endif

		previous_delay = delay_map[0]._run_delay;

	// !! Here we GO !!
	//(yes I'm quoting SFACT)

	for(uint16_t counter = 1; min_delay != delay_map[counter-1]._run_delay; ++counter)
	{
		// Calculate the delay
		delay_buffer = max(min_delay, previous_delay - (previous_delay * max_accel_s_mm_s));

		// See how many subsequent steps use this same delay.
		if(delay_buffer == delay_map[last_unique_value]._run_delay)
		{
			delay_map[last_unique_value]._runs_left ++;
		}
		else
		{
			last_unique_value ++;
			delay_map[last_unique_value]._run_delay = delay_buffer;
			++delay_map[last_unique_value]._runs_left;
		}

		previous_delay = delay_buffer;

	}

	//now me must populate the end of the array with the numbers from the start

	for(uint16_t counter = 1; last_unique_value - counter >= 0; counter ++)
	{
		//Huh, ChatterComa was right
		//it IS always good to have a copy constructor lying around.
		delay_map[last_unique_value + counter] = new DelayRun(&(delay_map[last_unique_value - counter]));
	}


	/* HERE LIES ONE OF THE GREATEST CODING FAILIURES EVER
	 * The line
	 * while(&(delay_map[last_unique_value - counter]) != &(delay_map[0]));
	 * was replaced by
	 * while(last_unique_value - counter > 0));
	 */



	#ifdef DEBUG_MOVEMENT
		Serial.print("Created an run-length acceleration array with ");
		Serial.print(required_array_length_start_end);
		Serial.println(" elements.");
	#endif

		return new Delayer(delay_map, total_delayer_calls);


}


///*-----------------------------------------------------------------------------
// old way
//-----------------------------------------------------------------------------*/
//Delayer * Delayer::factory(float min_delay_in_ms, uint16_t total_delayer_calls)
//{
//	// Validate arguments.
//	ASSERT(total_delayer_calls > 1);
//
//	ASSERT(min_delay_in_ms > 0.0);
//
//	DelayTimeInMicroseconds min_delay = min_delay_in_ms * 1000; //convert from ms to us
//
//	//make space for the delay_map
//	DelayTimeInMicroseconds * delay_map = new DelayTimeInMicroseconds[total_delayer_calls];
//	ASSERT(delay_map != 0);
//
//	#ifdef ACCELERATION_KILL
//		for(uint16_t counter = 1; counter < total_delayer_calls; ++counter)
//		{
//			//set everything to min_delay
//			delay_map[counter] = min_delay;
//		}
//		return return new Delayer(delay_map, total_delayer_calls);
//	#endif
//
//	//obtain needed value from Printer
//	uint32_t max_acceleration = Printer::instance().max_acceleration;
//
//
//	//make sure max_acceleration is > 1 mm/s/s
//	//or the formula will crash
//	ASSERT(max_acceleration > 1);
//
//
//	//convert from mm/s/s to s/mm/s
//	//this is where it gets weird, folks
//	float max_accel_s_mm_s = 1.0-(1.0/max_acceleration);
//
//	delay_map[0] = min_delay * ACCEL_START_SPEED_MULTIPLIER;
//
//	for(uint16_t counter = 1; counter < total_delayer_calls/2; ++counter)
//	{
//		// Delay frpom previous iteration.
//		DelayTimeInMicroseconds previous_delay = delay_map[counter - 1];
//
//
//		// The table entries we're updating this iteration.
//		DelayTimeInMicroseconds * front_cell_ptr = &delay_map[counter];
//		DelayTimeInMicroseconds * back_cell_ptr = &delay_map[total_delayer_calls - (counter + 1)];
//
//
//		// Take the previous delay and reduce it by an amount that's
//		// proportional to the acceleration.  The higher the acceleration,
//		// the more we reduce it.
//		*front_cell_ptr = *back_cell_ptr = max(min_delay, previous_delay - (previous_delay * max_accel_s_mm_s));
//	}
//
//	#ifdef DEBUG_MOVEMENT
//		Delayer* delayer = new Delayer(delay_map, total_delayer_calls);
//		delayer->debug_accelimap();
//		return delayer;
//	#else
//		return new Delayer(delay_map, total_delayer_calls);
//	#endif
//
//}


/*-----------------------------------------------------------------------------

-----------------------------------------------------------------------------*/
Delayer::Delayer(DelayRun * delay_map, uint16_t map_length)
:	_delay_map(delay_map),
 	_delay_map_length(map_length),
 	_global_counter(0)
{
}


/*-----------------------------------------------------------------------------

-----------------------------------------------------------------------------*/
void Delayer::operator ()()
{
	// Off the end of delay map?
	ASSERT(_global_counter < _delay_map_length);

	//the idea is that we find a run that is not 0,
	//if the current one has 0 uses left we move on.
	if(_delay_map[_global_counter]._runs_left == 0)
	{
		++_global_counter;
	}

	//since everything in _delay_map should have at least one use, this should never happen
	//if it does, then we have an off-by-one error somewhere
	ASSERT(_delay_map[_global_counter]._runs_left > 0)

	//now that we know the proper place, get one
	//get_delay() decreases runs_left
	DelayTimeInMicroseconds delay_time = _delay_map[_global_counter].get_delay();

	if(delay_time > 15000)
	{
		// Use this for "long" delays.
		delay(delay_time/ 1000);
	}
	else
	{
		// Use this for "short" delays.
		delayMicroseconds(delay_time);
	}
}

/*-----------------------------------------------------------------------------

-----------------------------------------------------------------------------*/
void Delayer::debug_accelimap()
{
	#ifdef DEBUG_MOVEMENT
		Serial.println("delay_map printout:");
		for(uint16_t counter = 0; counter < (_delay_map_length); ++counter)
		{
			Serial.print("[delay time: ");
			Serial.print(_delay_map[counter]._run_delay);
			Serial.print("] [run length:");
			Serial.print(_delay_map[counter]._runs_left);
			Serial.println("]");
		}

	#endif
}

/*-----------------------------------------------------------------------------

-----------------------------------------------------------------------------*/
Delayer::~Delayer()
{
	delete _delay_map;
}
