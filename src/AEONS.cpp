
/*-----------------------------------------------------------------------------
		   ____	    __ 	     		__
   /\     |       /    \   |\   |  /  \
  /  \    |____  |      |  | \  |  \___
 /----\   |      |      |  |  \ |      \
/      \  |____   \ __ /   |   \|   ___/
                                       ...because C++ isn't quite so bad after all.


Implemented Codes
-------------------
	Gcode:
	G0  -> G1
	G1  - Coordinated Movement X Y Z E
	G4  - Dwell S<seconds> or P<milliseconds>
	G28 - Home all Axis
	G90 - Use Absolute Coordinates
	G91 - Use Relative Coordinates
	G92 - Set assumed position to cordinates given

	Official RepRap M-Codes:
	M40 - Eject printed object (user specified commands)
	M104 - Set extruder target temp
	M105 - Read current temp
	M106 - Fan on
	M107 - Fan off
	M114 - Display current position

	Custom M-Codes:
	M80  - Turn on Power Supply
	M81  - Turn off Power Supply
	M84  - Disable steppers until next move, or use S<seconds> to specify an inactivity timeout, after which the steppers will be disabled.  S0 to disable the timeout.
	M140 - Set bed target temp
	M116 - Wait for extuder AND bed to heat up


To Be Implemented
-------------------

	RepRap M Codes

	M109 - Wait for extruder current temp to reach target temp

	Custom M Codes

	M42 - Set output on free pins, on a non pwm pin (over pin 13 on an arduino mega) use S255 to turn it on and S0 to turn it off.
		  Use P to decide the pin (M42 P23 S255) would turn pin 23 on
	M84  - Disable steppers until next move, or use S<seconds> to specify an inactivity timeout, after which the steppers will be disabled.  S0 to disable the timeout.
	M85  - Set inactivity shutdown timer with parameter S<seconds>. To disable set zero (default)
	M92  - Set axis_steps_per_unit - same syntax as G92
	M115 - Capabilities string
	M190 - Wait for bed current temp to reach target temp.

	Dual Extruder Support

	Tx - change extruder to x (starts at extruder)

	 By MultipleMonomials and ChatterComa, thx to Kliment
-----------------------------------------------------------------------------*/

#include "AEONS.h"
#include "String.h"
#include "Arduino.h"
#include <stdlib.h>
#include "Printer.h"
#include "gcodes.h"
#include "assert.h"
#include "UnitTest.h"
#include "Inactivity.h"
#include <assert.h>
#include "Parser.h"


/*-----------------------------------------------------------------------------
	Global variables.
-----------------------------------------------------------------------------*/

//last line number
long line_number = 0;

/*-----------------------------------------------------------------------------
-----------------------------------------------------------------------------*/

extern "C" void __cxa_pure_virtual() {}

void setup()
{

    // setup serial connection
	Serial.begin(BAUDRATE);
	Serial.println("start");

	//create and init device objects
	//which set up their own pins

	//init Printer object
	Printer::instance();

	//init inactivity timer
	Inactivity::instance();

	#ifdef RUN_UNIT_TESTS
	// Invoke unit tests
	TestAll();
	#endif
}

/*-----------------------------------------------------------------------------
-----------------------------------------------------------------------------*/
void manage_temperatures()
{
	#ifdef HAS_EXTRUDER
		Printer::instance().Extruder.manage_temperature();
	#endif

	#ifdef HAS_SECOND_EXTRUDER
		Printer::instance().Extruder_2.manage_temperature();
	#endif

	#ifdef HAS_BED
		Printer::instance().Bed.manage_temperature();
	#endif
}

/*-----------------------------------------------------------------------------
-----------------------------------------------------------------------------*/
void loop()
{
	manage_temperatures();
	if(Serial.available())
	{
		//Received a command, reset inactivity timer
		Inactivity::instance().restart();

		//read command from serial and put into Printer::instance().command
		get_next_command(Printer::instance().command, sizeof(Printer::instance().command));
		code * code_recieved = gcode_factory();
		if(code_recieved!=NULL)
		{
		//call the code object's process function and then free it from memory
			code_recieved->process();
			delete code_recieved;
		}

		clear_command(Printer::instance().command);
	}
	Inactivity::instance().check();
}



/*-----------------------------------------------------------------------------
	General-purpose routine for assembling characters from the serial port
	a newline is found.  Returns a null-terminated sting in the supplied
	buffer
-----------------------------------------------------------------------------*/
void get_next_command(char * buffer, int buffer_length)
{
	// Keep reading until we come upon a newline character.
	#ifdef DEBUG_GCODE_PARSING
		Serial.println("Read character: ");
	#endif

	int counter = 0;

	//request next command from host
	Serial.println("ok");

	do
	{
		if(counter >= buffer_length)
		{
			Serial.println("!! ERROR: Gcode buffer overflow.  Aborting program.");
			abort();
		}

		buffer[counter] = Serial.read();
		#ifdef DEBUG_GCODE_PARSING
			Serial.print(buffer[counter]);
		#endif
	}
	while (buffer[counter++] != '\n');


	// Convert to null-terminated string.
	buffer[counter-1] = '\0';
}

/*-----------------------------------------------------------------------------
-----------------------------------------------------------------------------*/
bool verify(int n_value, int checksum_from_command, char* command)
{
	int checksum = 0;
	for(int counter = 0; command[counter] != '*' && command[counter] != 0; counter++)
		checksum = checksum ^ command[counter];
	checksum &= 0xff;  // Defensive programming...

	if((n_value == 0 && line_number != 0) || checksum_from_command == 0)
	{
		return true; //the host does not support line checksums
	}

	if(n_value == line_number + 1 && checksum_from_command == checksum)
	{
		++line_number;
		return true;
	}

	else
	{
		Serial.print("rs"); //ask the host to resend the lost command
		Serial.println(" ");
		Serial.print(line_number + 1);
		return false;
	}
}

/*-----------------------------------------------------------------------------
Reads the command from Printer::instance().command and returns a code object of
the correct type and with the correct data.
-----------------------------------------------------------------------------*/
code * gcode_factory()
{

	remove_comments(Printer::instance().command);

	//next part sets variables to attributes of the recieved code

	bool has_g_value = test_for_char(Printer::instance().command, 'G');

	int g_value;
	if(has_g_value)
	{
		g_value = find_long_in_command(Printer::instance().command, 'G');
	}

	bool has_m_value = test_for_char(Printer::instance().command, 'M');

	int m_value;
	if(has_g_value)
	{
		m_value = find_long_in_command(Printer::instance().command, 'M');
	}

	bool has_t_value = test_for_char(Printer::instance().command, 'T');

	int t_value;
	if(has_t_value)
	{
		t_value = find_long_in_command(Printer::instance().command, 'T');
	}


	#ifdef VERIFY_GCODES
	int n_value = find_long_in_command(Printer::instance().command, 'N');
	#endif

	if(!(has_g_value || has_m_value || has_t_value))
		Serial.println("Error! Neither a g-value or an m-value were recieved!");

	#ifdef VERIFY_GCODES
	if(!verify(n_value, (int) find_double_in_command(Printer::instance().command, '*'), Printer::instance().command))
	{
		Serial.print("ERROR: Corrupt gcode \"");
		Serial.print(Printer::instance().command);
		Serial.println("\".  Checksum mismatch.");
		return NULL;
	}
	#endif

	//Now we construct the correct Gcode object
	if(has_g_value)
	{

		switch(g_value)
		{
			case 0:
			case 1:
				return new G1(Printer::instance().command);
				break;
			case 4:
				return new G4(Printer::instance().command);
				break;
			case 28:
				return new G28(Printer::instance().command);
				break;
			case 90:
				return new G90(Printer::instance().command);
				break;
			case 91:
				return new G91(Printer::instance().command);
				break;
			case 92:
				return new G92(Printer::instance().command);
				break;
		}
	}

	else if(has_m_value)
	{
		switch(m_value)
		{
			case 40:
				return new M40(Printer::instance().command);
				break;

			case 80:
				return new M80(Printer::instance().command);
				break;

			case 81:
				return new M81(Printer::instance().command);
				break;

			case 84:
				return new M84(Printer::instance().command);
				break;

			case 104:
				return new M104(Printer::instance().command);
				break;

			case 105:
				return new M105(Printer::instance().command);
				break;


			case 106:
				return new M106(Printer::instance().command);
				break;

			case 107:
				return new M107(Printer::instance().command);
				break;

			case 114:
				return new M114(Printer::instance().command);
				break;

			case 116:
				return new M116(Printer::instance().command);
				break;

			case 140:
				return new M140(Printer::instance().command);
				break;

			case 201:
			case 202:
				return new M201(Printer::instance().command);
				break;
		}
	}

	if(has_t_value)
	{
		//we're changing the extruder to extruder t_value
		//starts at extruder 0
		switch(t_value)
		{
			//default extruder
			#ifdef HAS_EXTRUDER
				case 0:
					Printer::instance().e_axis = Printer::instance().e_axis_0;
					Printer::instance().Extruder = Printer::instance().Extruder_1;
					break;
			#endif

			#ifdef HAS_SECOND_EXTRUDER
				case 1:
					Printer::instance().e_axis = Printer::instance().e_axis_1;
					Printer::instance().Extruder = Printer::instance().Extruder_2;
					break;

			#endif

			default:
				Serial.print("ERROR: Extruder ");
				Serial.print(t_value);
				Serial.print("Not found");
				break;
		}

	}

	Serial.println("ERROR: Gcode not found");
	return NULL;
}

/*-----------------------------------------------------------------------------
 * Return the temperature of the active extruder.
 * Only here because it needs to see both Printer and Heater
-----------------------------------------------------------------------------*/
unsigned int getCurrentExtruderTemperature()
{
	return Printer::instance().Extruder.getTemperature();
}

/*-----------------------------------------------------------------------------
-----------------------------------------------------------------------------*/
void init_pins()
{
	//Initialize Dir Pins
  #if X_DIR_PIN > -1
    pinMode(X_DIR_PIN, OUTPUT);
  #endif
  #if Y_DIR_PIN > -1
    pinMode(Y_DIR_PIN, OUTPUT);
  #endif
  #if Z_DIR_PIN > -1
    pinMode(Z_DIR_PIN, OUTPUT);
  #endif
  #if E_DIR_PIN > -1
    pinMode(E_DIR_PIN, OUTPUT);
  #endif

  #if (HEATER_0_PIN > -1)
    pinMode(HEATER_0_PIN, OUTPUT);
    digitalWrite(HEATER_0_PIN,LOW);
  #endif
  #if (HEATER_1_PIN > -1)
    pinMode(HEATER_1_PIN, OUTPUT);
    digitalWrite(HEATER_1_PIN,LOW);
  #endif

	//Initialize Fan Pin
  #if (FAN_PIN > -1)
    pinMode(FAN_PIN, OUTPUT);
  #endif

	//Initialize Alarm Pin
  #if (ALARM_PIN > -1)
    pinMode(ALARM_PIN, OUTPUT);
    digitalWrite(ALARM_PIN,LOW);
  #endif

	//Initialize LED Pin
  #if (LED_PIN > -1)
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN,LOW);
  #endif

	//Initialize Step Pins
  #if (X_STEP_PIN > -1)
    pinMode(X_STEP_PIN, OUTPUT);
  #endif
  #if (Y_STEP_PIN > -1)
    pinMode(Y_STEP_PIN, OUTPUT);
  #endif
  #if (Z_STEP_PIN > -1)
    pinMode(Z_STEP_PIN, OUTPUT);
  #endif
  #if (E_STEP_PIN > -1)
    pinMode(E_STEP_PIN, OUTPUT);
  #endif

}
