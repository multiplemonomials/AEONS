/*
 * DelayRun.h
 *
 *  Created on: May 21, 2013
 *      Author: Jamie
 */

/*-----------------------------------------------------------------------------
	DelayRun.h

	//uses run-length encoing to store a set of delays

	By MultipleMonomials and ChatterComa, thx to Kliment
-----------------------------------------------------------------------------*/

#ifndef DELAYRUN_H
#define DELAYRUN_H

#include "Arduino.h"

typedef uint32_t DelayTimeInMicroseconds;

/*-----------------------------------------------------------------------------
-----------------------------------------------------------------------------*/
class DelayRun
{
public:

	// Variable-length array of delay values.
	DelayTimeInMicroseconds 	 		_run_delay;

	// Length of this run.
	uint16_t 	 						_runs_left;

	// Default Ctor.
	DelayRun();


	// Ctor.
	DelayRun(DelayTimeInMicroseconds const & run_delay, uint16_t const & run_length);
	// assuming that no run is longer than 65,535

	// Copy Ctor.
	DelayRun(DelayRun * delay_run);


	// Returns the delay, or zero if this run has been exhausted.
	DelayTimeInMicroseconds get_delay();
};


#endif



