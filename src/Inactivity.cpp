/*-----------------------------------------------------------------------------
	Inactivity.cpp

	By MultipleMonomials and ChatterComa
-----------------------------------------------------------------------------*/

#include "AEONS_Config.h"
#include "Inactivity.h"
#include "Printer.h"

/*-----------------------------------------------------------------------------
	Instance variable of this singleton object.
-----------------------------------------------------------------------------*/
Inactivity * Inactivity::_instance = 0;


/*-----------------------------------------------------------------------------
	Get a reference to the single, system-wide instance of Inactivity.
-----------------------------------------------------------------------------*/
Inactivity & Inactivity::instance()
{
	if (_instance == 0)
	{
		_instance = new Inactivity();
	}

	return *_instance;
}


/*-----------------------------------------------------------------------------

-----------------------------------------------------------------------------*/
Inactivity::Inactivity()
	: last_activity_milliseconds(millis())
{
}


/*-----------------------------------------------------------------------------

-----------------------------------------------------------------------------*/
void Inactivity::clear()
{
	#if INACTIVITY_TIMEOUT_IN_SECONDS >= 1
		last_activity_milliseconds = millis();
	#endif
}


/*-----------------------------------------------------------------------------

-----------------------------------------------------------------------------*/
void Inactivity::check()
{
	#if INACTIVITY_TIMEOUT_IN_SECONDS >= 1

	//millis resets itself after about 50 days, and we don't want to crash, so
		if(last_activity_milliseconds > millis())
		{
			clear();
			return;
		}

		if(last_activity_milliseconds + (unsigned long)(INACTIVITY_TIMEOUT_IN_SECONDS * 1000) < (unsigned long)millis())
		{
			// Time has not expired.
			return;
		}


		// Time's up!

		#ifdef WARN_HOST_ON_SHUTDOWN
			Serial.println("// Inactivity shutdown activated!");
		#endif

		#ifdef SET_TEMPS_TO_0_ON_SHUTDOWN
			#ifdef HAS_EXTRUDER
				Printer::instance().Extruder.setTemperature(0);
			#endif

			#ifdef HAS_SECOND_EXTRUDER
				Printer::instance().Extruder_2.setTemperature(0);
			#endif

			#ifdef HAS_BED
				Printer::instance().Bed.setTemperature(0);
			#endif
		#endif

		#ifdef TURN_OFF_POWER_SUPPLY_ON_SHUTDOWN
			Printer::instance().Power_Supply.turn_off();
		#endif

	#endif
}

