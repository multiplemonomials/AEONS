/*-----------------------------------------------------------------------------
	Pin.h

	By MultipleMonomials and ChatterComa, thx to Kliment
-----------------------------------------------------------------------------*/
#ifndef PIN_H
#define PIN_H
#include "Arduino.h"
#include "AEONS_Typedefs.h"
#include "assert.h"

/* for reference
#define HIGH 0x1
#define LOW  0x0

#define INPUT 0x0
#define OUTPUT 0x1
#define INPUT_PULLUP 0x2

#define true 0x1
#define false 0x0
*/

/*-----------------------------------------------------------------------------

-----------------------------------------------------------------------------*/
class DigitalInputPin
{
	Pin _pin_number;

public:
	// Ctor.
	DigitalInputPin(Pin pin_number, bool enable_pullups=true);

	// Returns true if pin is in a high state.
	bool isActive();
};


/*-----------------------------------------------------------------------------

-----------------------------------------------------------------------------*/
class DigitalOutputPin
{
	Pin _pin_number;
	unsigned int _pulse_duration;
	bool _active_high;

public:
	// Ctor.
	DigitalOutputPin(Pin pin_number, uint16_t pulseDuration, bool active_high=true);

	// Set the pin output active.
	void setActive();

	void setInactive();

	// Pulses the pin.
	void pulse();
};


/*-----------------------------------------------------------------------------

-----------------------------------------------------------------------------*/
class AnalogInputPin
{
	Pin _pin_number;
	bool _active_high;

public:
	// Ctor.
	AnalogInputPin(Pin pin_number, bool active_high=true);

	// Returns the current value of the pin between 0..255.
	int get_value();
};

/*-----------------------------------------------------------------------------

-----------------------------------------------------------------------------*/
class AnalogOutputPin
{
	Pin _pin_number;

public:
	// Ctor.
	AnalogOutputPin(Pin pin_number);

	// Set the pin output to between 0..255.
	void set_value(uint8_t value);
};

#endif /* PIN_H */
