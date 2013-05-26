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
	Provides scripted delayes for use during a move operation.  Factory
	service calcualtes acceleration values, then provides a pre-computed
	table of delay values which are used in successive calls to operator ().
-----------------------------------------------------------------------------*/
class Delayer
{
private:

	// Variable-length array of delay values.
	DelayRun * _delay_map;

	// Length of above.
	uint16_t _delay_map_length;

	// Number of calls to operator() so far.
	uint16_t _global_counter;

	// Ctor.  Private since this object is always factory-built.
	Delayer(DelayRun * delay_map, uint16_t map_length);


public:

	// Creates and returns a new Delayer object using the specified information.
	static Delayer * factory(float min_delay, uint16_t total_delayer_calls);

	// Call to provide the appropriate delay.
	void operator ()();

	void debug_accelimap();

	// Dtor.  Needed to free delay_map from heap.
	~Delayer();
};


#endif
