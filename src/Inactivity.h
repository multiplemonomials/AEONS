/*-----------------------------------------------------------------------------
	Inactivity.h

	By MultipleMonomials and ChatterComa
-----------------------------------------------------------------------------*/
#ifndef INACTIVITY_H
#define INACTIVITY_H

#include "Arduino.h"

/*-----------------------------------------------------------------------------
Singleton object that takes a recovery action if too much time
has passed since clear() was called.  Usage:

	Inactivity::instance();  // construct.

	Inactivity::instance().clear(); // start of time period.

	while(...)
	{
		Inactivity::instance().check();  // check for expiration.
	}


Works with the serial communucations code
to figure out if the printer has been on past the time limit
then shuts it down if it has
-----------------------------------------------------------------------------*/
class Inactivity
{
private:

	// Time in mS since last clear().
	unsigned long _last_activity_milliseconds;

	// Reference to the One True Object.
	static Inactivity * _instance;

	unsigned long _deadline_milliseconds;

	// Private in this singleton object.
	Inactivity();

public:

	// Get a reference to the single, system-wide instance of Inactivity.
	static Inactivity & instance();

	// Restart the time period.
	void restart();

	// See if too much time has elapsed and take action if so.
	void check();
};

#endif
