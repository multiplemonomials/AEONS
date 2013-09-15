/*-----------------------------------------------------------------------------
	Delayer.h

	To use:

	(1) Call the factory to obtain the appropriate functor for the specified Delayer:

		delay_base * myDelayer = delay_base::delay_factory(2000.0);

	(2) Invoke the Delayer as needed.

		(*myDelayer)();

	(3) Free the functor when done.

		delete myDelayer;


	By MultipleMonomials and ChatterComa, thx to Kliment
-----------------------------------------------------------------------------*/

#ifndef DELAY_H
#define DELAY_H

#include "Arduino.h"
#include "DelayRun.h"

typedef uint32_t DelayTimeInMicroseconds;

/*-----------------------------------------------------------------------------
	Calculates variable delay times to use in each print head move.
	Implements print head acceleration and deceleration.
-----------------------------------------------------------------------------*/
class Delayer
{
private:

	// Delay value returned by previous operator() call.
	float _previous_delay;

	// Delay value returned at maximum print head speed.
	float _min_delay_ms;

	// Acceleration to use for current move.
	float _max_accel_s_mm_s;

	// The number of times the delay function has been called so far.
	uint16_t delayer_calls_so_far;

	// The total number of times that operator() wil be called on this move.
	uint16_t total_delayer_calls;

	// Holds the point at which the deceleration begins.
	uint16_t decel_location;

	// Number of operator() calls that bring us to the half-way point of the move.
	uint16_t halfway_point;

	float _max_accel_ms_mm_step;

	// Move can be in one of three states:  Accelerating, steady-state at
	// maximum spee, or decelerating.  The operator() function does different
	// sets of calculations depending on the current state.
	enum States
	{
		ACCEL,
		MAX_SPEED,
		DECEL
	};

	// Current state of the move.
	States state;


public:

	// Ctor.  Private since this object is always factory-built.
	Delayer(uint32_t delayer_calls, float min_delay, float delay_sec_step);

	// Call to provide the appropriate delay.
	void operator ()();

	void debug_accelimap();
};


#endif
