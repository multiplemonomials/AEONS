/*-----------------------------------------------------------------------------
	delay.cpp

	Uses functors (aka operator ()) to create an object that will delay the proper
	amount of time when the <name>() funtion is called.

	By MultipleMonomials and ChatterComa, thx to Kliment
-----------------------------------------------------------------------------*/

#include "delay.h"
#include "Arduino.h"

/*-----------------------------------------------------------------------------

-----------------------------------------------------------------------------*/
delay_base* delay_base::delay_factory(float delay_time)
{
	if(delay_time < 10.0)
	{
		return new delay_microseconds(delay_time);
	}
	else
	{
		return new delay_milliseconds(delay_time);
	}
}

///*-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------*/
delay_base::~delay_base()
{

}

/*-----------------------------------------------------------------------------

-----------------------------------------------------------------------------*/
void delay_milliseconds::operator ()()
{
	delay(saved_delay);
}

/*-----------------------------------------------------------------------------

-----------------------------------------------------------------------------*/
delay_milliseconds::delay_milliseconds(float delay_time)
{
	saved_delay = (int)delay_time;
}

/*-----------------------------------------------------------------------------

-----------------------------------------------------------------------------*/
delay_microseconds::delay_microseconds(float delay_time)
{
	saved_delay = (int)((delay_time * 1000.0)); //-46 is to account for length of movement code
}

/*-----------------------------------------------------------------------------

-----------------------------------------------------------------------------*/
void delay_microseconds::operator ()()
{
	delayMicroseconds(saved_delay);
}

