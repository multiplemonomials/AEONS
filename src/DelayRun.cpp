/*
 * DelayRun.cpp
 *
 *  Created on: May 21, 2013
 *      Author: Jamie
 */

#include "DelayRun.h"

/*-----------------------------------------------------------------------------
-----------------------------------------------------------------------------*/
DelayRun::DelayRun()
: _run_delay(0),
  _runs_left(0)
{

}


/*-----------------------------------------------------------------------------
-----------------------------------------------------------------------------*/
DelayRun::DelayRun(DelayTimeInMicroseconds const & run_delay, uint16_t const & run_length)
: _run_delay(run_delay),
  _runs_left(run_length)
{

}


/*-----------------------------------------------------------------------------
-----------------------------------------------------------------------------*/
DelayRun::DelayRun(DelayRun * delay_run)
: _run_delay(delay_run->_run_delay),
  _runs_left(delay_run->_runs_left)
{

}


/*-----------------------------------------------------------------------------
 * decreases the run length by one and also returs the delay
 * JUST READ THE VARIABLE IF YOU DO NOT WANT TO DECREASE IT!
-----------------------------------------------------------------------------*/
DelayTimeInMicroseconds DelayRun::get_delay()
{
	// Return _run_delay, or zero if the run has been exhausted.
	return (_runs_left-- > 0) ? _run_delay : 0;
}

