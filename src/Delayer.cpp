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


Delayer::Delayer(uint32_t delayer_calls, float min_delay)
 	:	_previous_delay(min_delay * (float)ACCEL_START_SPEED_MULTIPLIER),
 	_min_delay(min_delay),
 	delayer_calls_so_far(1),
	total_delayer_calls(delayer_calls),
 	decel_location(0),
 	halfway_point(delayer_calls / 2),
 	state(ACCEL)
{
	int max_acceleration_mm_s_s = Printer::instance().max_acceleration;

	//make sure max_acceleration is > 1 mm/s/s
		//or the formula will crash
		ASSERT(max_acceleration_mm_s_s > 1);


		//convert from mm/s/s to s/mm/s
		//this is where it gets weird, folks
		_max_accel_s_mm_s = 1.0-(1.0/(float)max_acceleration_mm_s_s);

	#ifdef DEBUG_MOVEMENT
			Serial.print("Max Acceleration: ");
			Serial.print(_max_accel_s_mm_s);
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

	float delay_time;
	switch(state)
	{
		case ACCEL:
		{
			delay_time = max(_min_delay, (float)_previous_delay - ((float)_previous_delay * _max_accel_s_mm_s));

			//if we've reached maximum speed, stay there;
			if(delay_time == _min_delay)
			{
				state = MAX_SPEED;
				decel_location = total_delayer_calls - delayer_calls_so_far;
			}
			//If we are past halfway, start decelerating.
			else if(delayer_calls_so_far > halfway_point)
			{
				state = DECEL;
				delay_time = _previous_delay + (_previous_delay * _max_accel_s_mm_s);
			}
		#ifdef DEBUG_MOVEMENT
			Serial.print("State: ACCEL");
			Serial.print("\n");
		#endif
			break;
		}
		case MAX_SPEED:
		{
			// If we're past the deceleration location, start slowing down.
			if(delayer_calls_so_far >= decel_location)
			{
				state = DECEL;
				delay_time = _previous_delay + (_previous_delay * _max_accel_s_mm_s);
			}
			// Otherwise just keep going at max speed.
			else
			{
				delay_time = _min_delay;
			}
		#ifdef DEBUG_MOVEMENT
			Serial.print("State: MAX_SPEED");
			Serial.print("\n");
		#endif
			break;
		}
		case DECEL:
		{
			Serial.print("Call # ");
			Serial.println(delayer_calls_so_far);
			ASSERT(total_delayer_calls >= delayer_calls_so_far)
			delay_time = _previous_delay + (_previous_delay * _max_accel_s_mm_s);
			break;
		}
		default:
		{
			ASSERT(false);


			break;
		}
	}

	++delayer_calls_so_far;

	#ifdef DEBUG_MOVEMENT
	Serial.print("Delay Time: ");
	Serial.print(delay_time);
	Serial.print("\n");
	#endif


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

	_previous_delay = delay_time;
}
