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


	Delayer::Delayer(uint32_t delayer_calls, float min_delay_ms, float delay_seconds_per_step)
 	:	_previous_delay(min_delay_ms * (float)ACCEL_START_SPEED_MULTIPLIER),
 	_min_delay_ms(min_delay_ms),
 	delayer_calls_so_far(1),
	total_delayer_calls(delayer_calls),
 	decel_location(0),
 	halfway_point(delayer_calls / 2),
 	state(ACCEL)
{
	// Obtain the max mechanical acceleration the user has set (mm/s/s).
	int max_acceleration_mm_s_s = Printer::instance().max_acceleration;


	// Make sure max_acceleration is > 1 mm/s/s or the formula will crash.
	ASSERT(max_acceleration_mm_s_s > 1);


	// Convert from mm/s/s to s/mm/s.
	_max_accel_s_mm_s = 1.0 - (1.0 / (float)max_acceleration_mm_s_s);




	// Convert to acceleration in ms/mm/s.
	float max_accel_ms_mm_s = _max_accel_s_mm_s * 1000.0;

	// Convert to per-step metrics.
	_max_accel_ms_mm_step = max_accel_ms_mm_s * delay_seconds_per_step;


	#ifdef DEBUG_MOVEMENT
		Serial.print("_max_accel_s_mm_s: ");
		Serial.print(_max_accel_s_mm_s);
		Serial.print("\n");
		Serial.print("max_accel_ms_mm_s: ");
		Serial.print(max_accel_ms_mm_s);
		Serial.print("\n");
		Serial.print("_max_accel_ms_mm_step: ");
		Serial.print(_max_accel_ms_mm_step);
		Serial.print("\n");
		Serial.print("delay_seconds_per_step: ");
		Serial.print(delay_seconds_per_step);
		Serial.print("\n");
		Serial.print("Previous Delay: ");
		Serial.print(_previous_delay);
		Serial.print("\n");
	#endif

}


/*-----------------------------------------------------------------------------

-----------------------------------------------------------------------------*/
void Delayer::operator ()()
{
	#ifdef DEBUG_MOVEMENT_STEPS
		uint32_t time_at_start = millis();
	#endif
	float delay_time_in_ms;
#ifndef ACCELERATION_KILL
	switch(state)
	{
		case ACCEL:
		{
			delay_time_in_ms = max(_min_delay_ms, (float)_previous_delay - ((float)_previous_delay * _max_accel_ms_mm_step));

			//if we've reached maximum speed, stay there;
			if(delay_time_in_ms == _min_delay_ms)
			{
				state = MAX_SPEED;
				decel_location = total_delayer_calls - delayer_calls_so_far;
			}
			//If we are past halfway, start decelerating.
			else if(delayer_calls_so_far > halfway_point)
			{
				state = DECEL;
				delay_time_in_ms = _previous_delay + (_previous_delay * _max_accel_ms_mm_step);
			}
		#ifdef DEBUG_MOVEMENT_STEPS
			Serial.print("State: ACCEL ");
		#endif
			break;
		}
		case MAX_SPEED:
		{
			// If we're past the deceleration location, start slowing down.
			if(delayer_calls_so_far >= decel_location)
			{
				state = DECEL;
				delay_time_in_ms = _previous_delay + (_previous_delay * _max_accel_ms_mm_step);
			}
			// Otherwise just keep going at max speed.
			else
			{
				delay_time_in_ms = _min_delay_ms;
			}
		#ifdef DEBUG_MOVEMENT_STEPS
			Serial.print("State: MAX_SPEED ");
		#endif
			break;
		}
		case DECEL:
		{
			ASSERT(total_delayer_calls >= delayer_calls_so_far)
			delay_time_in_ms = _previous_delay + (_previous_delay * _max_accel_ms_mm_step);
			#ifdef DEBUG_MOVEMENT_STEPS
				Serial.print("State: DECEL ");
			#endif
			break;
		}
		default:
		{
			ASSERT(false);


			break;
		}
	}

	++delayer_calls_so_far;

#else
	delay_time_in_ms = _min_delay_ms;
#endif

	#ifdef DEBUG_MOVEMENT_STEPS
	Serial.print("Delay Time: ");
	Serial.print(delay_time_in_ms);
	Serial.print(" ");
	#endif


	// Need to produce the delay using different services depending
	// on length.  15mS is the threshold.
	if(delay_time_in_ms > 15.0 /* mS */)
	{
		// Use this for "long" delays.
		delay((unsigned long) delay_time_in_ms);
	}
	else
	{
		// Use this for "short" delays.
		delayMicroseconds((unsigned int) ((float) delay_time_in_ms * 1000.0));
	}

	_previous_delay = delay_time_in_ms;

	#ifdef DEBUG_MOVEMENT_STEPS
		Serial.print("Step Time ");
		Serial.print(millis() - time_at_start);
		Serial.println("milliseconds");
	#endif
}
