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
{
	// Start the timer.
	restart();
}


/*-----------------------------------------------------------------------------

-----------------------------------------------------------------------------*/
void Inactivity::restart()
{
	#if INACTIVITY_TIMEOUT_IN_SECONDS >= 1
		_last_activity_milliseconds = millis();
		_deadline_milliseconds = _last_activity_milliseconds + ((unsigned long)INACTIVITY_TIMEOUT_IN_SECONDS * (unsigned long)1000);

		#ifdef DEBUG_INACTIVITY
			Serial.print("_last_activity_milliseconds = ");
			Serial.println(_last_activity_milliseconds);

			Serial.print("_deadline_milliseconds = ");
			Serial.println(_deadline_milliseconds);
		#endif
	#endif
}


/*-----------------------------------------------------------------------------

-----------------------------------------------------------------------------*/
void Inactivity::check()
{
	#if INACTIVITY_TIMEOUT_IN_SECONDS >= 1

		// Guard against millis() overflow (millis resets itself after about 50 days, and we don't want to crash)
		if(_last_activity_milliseconds > millis())
		{
			restart();
			return;
		}


		// Return if inactivity timeout has not expired.
		if(millis() < _deadline_milliseconds)
		{
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

		restart();

	#endif
}

