/*-----------------------------------------------------------------------------
	delay.h

	To use:

	(1) Call the factory to obtain the appropriate functor for the specified delay:

		delay_base * myDelayer = delay_base::delay_factory(2000.0);

	(2) Invoke the delay as needed.

		(*myDelayer)();

	(3) Free the functor when done.

		delete myDelayer;


	By MultipleMonomials and ChatterComa, thx to Kliment
-----------------------------------------------------------------------------*/

#ifndef DELAY_H
#define DELAY_H

/*-----------------------------------------------------------------------------
	Base class for a family of functor objects that implement measured delays.
-----------------------------------------------------------------------------*/
class delay_base
{
protected:

	int saved_delay;

public:

	virtual void operator ()();

	static delay_base* delay_factory(float delay_time);

//	virtual ~delay_base(){}
};


/*-----------------------------------------------------------------------------

-----------------------------------------------------------------------------*/
class delay_milliseconds : public delay_base
{
public:

	// Ctor.  Implement the delay in mS.
	delay_milliseconds(float delay_time);

	// Implement the specified delay.
	void operator ()();

	virtual ~delay_milliseconds(){}
};


/*-----------------------------------------------------------------------------

-----------------------------------------------------------------------------*/
class delay_microseconds : public delay_base
{
public:

	// Ctor.  Implement the delay in mS.
	delay_microseconds(float delay_time);

	void operator ()();

	virtual ~delay_microseconds(){}

};



#endif
