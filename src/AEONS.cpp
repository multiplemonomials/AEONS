
/*-----------------------------------------------------------------------------
		   ____						__
   /\     |         __     |\   |  /  \
  /  \    |____   /    \   | \  |  \___
 /----\   |      |      |  |  \ |      \
/      \  |____   \ __ /   |   \|   ___/
                                       ...because C++ isn't quite so bad after all.


Implemented Codes
-------------------
	Gcode:
	G0  -> G1
	G1  - Coordinated Movement X Y Z E
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

	Custom M-Codes:
	M80  - Turn on Power Supply
	M81  - Turn off Power Supply
	M140 - Set bed target temp
	M116 - Wait for extuder AND bed to heat up
	M222 - Report assued position


To Be Implemented
-------------------
	G4  - Dwell S<seconds> or P<milliseconds>

	RepRap M Codes

	M109 - Wait for extruder current temp to reach target temp.
	M114 - Display current position

	Custom M Codes

	M42 - Set output on free pins, on a non pwm pin (over pin 13 on an arduino mega) use S255 to turn it on and S0 to turn it off.
		  Use P to decide the pin (M42 P23 S255) would turn pin 23 on
	M84  - Disable steppers until next move,
			or use S<seconds> to specify an inactivity timeout, after which the steppers will be disabled.  S0 to disable the timeout.
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


/*-----------------------------------------------------------------------------
	Global variables.
-----------------------------------------------------------------------------*/

int current_x_position = 0;
int current_y_position = 0;
int current_z_position = 0;
int current_e_position = 0;
int current_f_position = 0;

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

	//initialize pins
	init_pins();

	//init Priinter object
	Printer::instance();

	#ifdef RUN_UNIT_TESTS
	// Invoke unit tests
	TestAll();
	#endif

	//Debugging stuff
//	Serial.println("Stepping Z-Axis");
//	Printer::instance().x_axis.enable();
//	Printer::instance().y_axis.enable();
//	Printer::instance().z_axis.enable();
//	Printer::instance().e_axis.enable();
//
//	while(true)
//	{
//		Printer::instance().z_axis.step();
//		delay(1);
//	}
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
		get_next_command(Printer::instance().command, sizeof(Printer::instance().command));
		code * code_recieved = gcode_factory();
		if(code_recieved!=NULL)
		{
		//call the code object's process function and then free it from memory
			code_recieved->process();
			delete(code_recieved);
		}

		clear_command();
	}
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
		buffer[counter] = Serial.read();
		#ifdef DEBUG_GCODE_PARSING
			Serial.print(buffer[counter], " ");
		#endif
		//stop us going TOO fast and reading before we have any more characters to read
		delay(2);
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
Reads one command from Printer.command and returns a code object of the correct type
and with the correct data.
-----------------------------------------------------------------------------*/
code * gcode_factory()
{

	// Get next GCode command.

	fix_comments(Printer::instance().command);

	//next part sets variables to attributes of the recieved code
	//cast here should be OK since mcodes and gcodes are never decimals
	float g_value_temp_float;
	bool has_g_value = get_value_from_char_array_bool(Printer::instance().command, 'G', &g_value_temp_float);
	int g_value = (int) g_value_temp_float;


		bool has_m_value;
		int m_value = (int)get_value_from_char_array(Printer::instance().command, 'M');
		if(m_value != 0.0)
		{
			has_m_value = true;
		}

	#ifdef VERIFY_GCODES
	int n_value = (int) get_value_from_char_array(Printer::instance().command, 'N');
	#endif

	if(!(has_g_value || has_m_value))
		Serial.println("Error! Neither a g-value or an m-value were recieved!");

	#ifdef VERIFY_GCODES
		if(!verify(n_value, (int) get_value_from_char_array(Printer::instance().command, '*'), Printer::instance().command))
		{
			return NULL; //if we're here the gcode is corrupt
		}
	#endif

	#ifdef DEBUG_GCODE_PARSING
		Serial.print("Parsed gcode details:");
		Serial.print("G-value: ");
		Serial.println(g_value);
		Serial.print("M-value: ");
		Serial.println(M_value);
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
		}
	}
	float t_value;

	if(get_value_from_char_array_bool(Printer::instance().command, 'T', &t_value))
	{
		//we're changing the extruder to extruder t_value]
		//starts at extruder 0
		switch((uint16_t)t_value)
		{
			//default extruder
			#ifdef HAS_EXTRUDER
				case 0:
					Printer::instance().e_axis = Printer::instance().e_axis_0;
					break;
			#endif

			#ifdef HAS_SECOND_EXTRUDER
				case 1:
					Printer::instance().e_axis = Printer::instance().e_axis_1;
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
	Returns 0.0 if the specified character is not found in the char[]
	Otherwise returns the number that comes after target in code
-----------------------------------------------------------------------------*/
double get_value_from_char_array(char * code, char target)
{
	char * pointer_to_target = strchr(code, target);

	if (pointer_to_target == 0)
	{
		return 0;
	}

	char * end_ptr; // strtod returns a double (and a char*) from a char*
	double code_value = strtod((pointer_to_target + 1), &end_ptr);

	return code_value;
}

/*-----------------------------------------------------------------------------
	Returns false if the specified character is not found in the char[].
	This one is used by functions where an argument of 0 is different from
	no argument at all, pretty much just G1 and G92.
-----------------------------------------------------------------------------*/
bool get_value_from_char_array_bool(char * code, char target, float * return_value)
{
	char * pointer_to_target = strchr(code, target);

	if (pointer_to_target == 0)
	{
		*return_value = 0;
		return false;
	}

	char * end_ptr; // strtod returns a double (and a char*) from a char*
	*return_value = static_cast<float>(strtod((pointer_to_target + 1), &end_ptr));

	return true;
}

/*-----------------------------------------------------------------------------
	Returns false if the specified character is not found in the char[].
-----------------------------------------------------------------------------*/
bool test_for_char(char * code, char target)
{
	char * pointer_to_target = strchr(code, target);

	return pointer_to_target != 0;
}

/*-----------------------------------------------------------------------------
-----------------------------------------------------------------------------*/
void fix_comments(char * command)
{
	// Find comment delimiter if any.
	char * semicolon_ptr = 	strchr(command, ';');

	// Truncate command at semicolon.
	if (semicolon_ptr != 0)
	{
		*semicolon_ptr = '\0';
	}
}

/*-----------------------------------------------------------------------------
Sets every member of command to null so it can be reused.
-----------------------------------------------------------------------------*/
void clear_command()
{
		Printer::instance().command[0] = '\0';
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
