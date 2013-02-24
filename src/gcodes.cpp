/*-----------------------------------------------------------------------------
	gcodes.cpp
	Code file fr all of the gcode objects
	By MultipleMonomials and ChatterComa, thx to Kliment
-----------------------------------------------------------------------------*/

#include "gcodes.h"


/*-----------------------------------------------------------------------------
-----------------------------------------------------------------------------*/

G1::G1(char * command)
{
	// Parse the command and extract parameters if present.
	// If we didn't recieve an argument, "move" to the current position
	if(!get_value_from_char_array_bool(command, 'X', &x_value))
	{
		x_value = Printer::instance().x_axis._current_position;
	}

	if(!get_value_from_char_array_bool(command, 'Y', &y_value))
	{
		y_value = Printer::instance().y_axis._current_position;
	}

	if(!get_value_from_char_array_bool(command, 'Z', &z_value))
	{
		z_value = Printer::instance().z_axis._current_position;
	}

	if(!get_value_from_char_array_bool(command, 'E', &e_value))
	{
		e_value = Printer::instance().e_axis._current_position;
	}

	// Zero returned if F not supplied in command.
	f_value = get_value_from_char_array(command, 'F');
	if (f_value == 0.0)
	{
		//an f value was not included in the command, so use the last provided one
		f_value = Printer::instance().last_feedrate;
	}
}

void G1::process()
{
	//-------------------------------------------------------------------------------
	//if we are in absolute mode, convert to relative coodinates
	//-------------------------------------------------------------------------------
	if(!Printer::instance().relative_mode)
	{
		x_value = x_value - Printer::instance().x_axis.getCurrentPosition();
		y_value = y_value - Printer::instance().y_axis.getCurrentPosition();
		z_value = z_value - Printer::instance().z_axis.getCurrentPosition();
		e_value = e_value - Printer::instance().e_axis.getCurrentPosition();
	}


	//void move(float x_target, float y_target, float z_target, float e_target, int feedrate, bool absolute_mode)
	move(x_value, y_value, z_value, e_value, f_value);
}

/*-----------------------------------------------------------------------------
G90 Set absolute positioning
-----------------------------------------------------------------------------*/
G90::G90(char * command)
{
	// TBD
}

void G90::process()
{
	Printer::instance().relative_mode = false;
}

/*-----------------------------------------------------------------------------
G91 Set relative positioning
-----------------------------------------------------------------------------*/
G91::G91(char * command)
{
	// TBD
}

void G91::process()
{
	Printer::instance().relative_mode = true;
}

/*-----------------------------------------------------------------------------
M40--Eject printed objects by running user-specified commands
-----------------------------------------------------------------------------*/
M40::M40(char* command)
{


}

void M40::process()
{

	int line_counter = 0;

	#ifdef DEBUG_GCODE_PROCESSING
		Serial.print("M40 started.  Operating on line:");
		Serial.println(Printer::instance().m40_commands);
	#endif

	for(uint16_t counter = 0; counter < sizeof(Printer::instance().m40_commands);counter ++)
	{
		//if the current char is not a newline or a null, set the corresponding place in the input buffer to it
		if 	((Printer::instance().m40_commands[counter] != '\n') && (Printer::instance().m40_commands[counter] != '\0'))
		{
			Printer::instance().command[counter] = 	Printer::instance().m40_commands[counter];

			#ifdef DEBUG_GCODE_PROCESSING
				Serial.print("M40::Process(): Char: ");
				Serial.print(Printer::instance().m40_commands[counter]);
				Serial.println(" Is not a newline. \nAdding to command...");
			#endif

		}

		//if it IS a newline, call gcode_factory on the command
		else if(Printer::instance().m40_commands[counter] == '\n')
		{
			#ifdef DEBUG_GCODE_PROCESSING
				Serial.print("M40::Process: Calling gcode factory on line ");
				Serial.print(++line_counter);
				Serial.print(" of char*:");
				Serial.print(Printer::instance().m40_commands);
			#endif

				code* code_to_process = gcode_factory();

				if (code_to_process != 0)
					code_to_process->process();

				clear_command();
		}

		//the for loop SHOULD stop before this triggers but it's here just in case
		else if(Printer::instance().m40_commands[counter] == '\0')
		{
			break;
		}

	}

}

/*-----------------------------------------------------------------------------
M80 Turn On Power Supply
-----------------------------------------------------------------------------*/	

M80::M80(char * command)
{
	// TBD
}

void M80::process()
{
	#ifdef HAS_POWER_SUPPLY
		Printer::instance().Power_Supply.turn_on();
	#endif
}

/*-----------------------------------------------------------------------------
M81 Turn Off Power Supply
-----------------------------------------------------------------------------*/

M81::M81(char * command)
{
	// TBD
}

void M81::process()
{
	#ifdef HAS_POWER_SUPPLY
		Printer::instance().Power_Supply.turn_off();
	#endif

	#ifdef DEBUG_GCODE_PROCESSING
			Serial.println("Recieved M80, turning off power supply...");
	#endif
}

/*-----------------------------------------------------------------------------
M104 (Sxxx) Set extruder temperature to given temp
-----------------------------------------------------------------------------*/

M104::M104(char * command)
{
	s_value =(int) get_value_from_char_array(command, 'S');
	if(s_value == 0)
	{
		Serial.println("No argument (M104 Sxxx) provided for M104, setting temp to 0...");
	}
	
}

// Will set temp to 0 if argument not found, I consider this good.

void M104::process()
{
	#ifdef HAS_EXTRUDER
		Printer::instance().Extruder.setTemperature(s_value);
	#endif
}

/*-----------------------------------------------------------------------------
M105 Turn On Power Supply
-----------------------------------------------------------------------------*/	

M105::M105(char * command)
{
	// TBD
}

void M105::process()
{
	#ifdef HAS_EXTRUDER
		Serial.print("ok\n T:");
		Serial.print(Printer::instance().Extruder.getTemperature()); 
	#endif
	#ifdef HAS_BED
		Serial.print(" B:");
		Serial.println(Printer::instance().Bed.getTemperature());
	#else
		Serial.print('\n');
	#endif
}


/*-----------------------------------------------------------------------------
M106 Turn On Fan
-----------------------------------------------------------------------------*/

M106::M106(char * command)
{
	// TBD
}

void M106::process()
{
	#ifdef HAS_POWER_SUPPLY
		Printer::instance().Fan.turn_on();
	#endif
}

/*-----------------------------------------------------------------------------
M107 Turn Off Fan
-----------------------------------------------------------------------------*/

M107::M107(char * command)
{
	// TBD
}

void M107::process()
{
	#ifdef HAS_POWER_SUPPLY
		Printer::instance().Fan.turn_off();
	#endif
}

/*------------------------------------------------------------------------------------
M116 [Ssss] [Pppp]-- Wait for extruder and bed to heat up (can specify temps for both)
------------------------------------------------------------------------------------*/

M116::M116(char * command)
{
	//user can optionally set temp values
	s_value =(int) get_value_from_char_array(command, 'S');
	p_value =(int) get_value_from_char_array(command, 'P');


}

void M116::process()
{
	bool extruder_up_to_temp =
	#ifdef HAS_EXTRUDER
		false;
	#else
		true;
	#endif

	//If we don't have a bed we'll just say it's already hot
	bool bed_up_to_temp =
	#ifdef HAS_BED
		false;
	#else
		true;
	#endif

	do
	{
		//not too cold
		#ifdef HAS_EXTRUDER
			extruder_up_to_temp =
					(Printer::instance().Extruder.getTemperature() >= (Printer::instance().Extruder.getTarget() - TEMPDELTA)) &&
					(Printer::instance().Extruder.getTemperature() <= (Printer::instance().Extruder.getTarget() + TEMPDELTA));
			Serial.print("ok T:");
			Serial.print(Printer::instance().Extruder.getTemperature());
		#endif

		//calling Extruder.getTemperature automatically manages the temperature
		#ifdef HAS_BED
			bed_up_to_temp =
					(Printer::instance().Bed.getTemperature() >= (Printer::instance().Bed.getTarget() - TEMPDELTA)) &&
					(Printer::instance().Bed.getTemperature() <= (Printer::instance().Bed.getTarget() + TEMPDELTA));
			Serial.print(" B:");
			Serial.println(Printer::instance().Bed.getTemperature());
		#endif

		delay(2000);
	}
	while(!(extruder_up_to_temp && bed_up_to_temp));

}

/*-----------------------------------------------------------------------------
M140 <Ssss>--Set bed temperature to given temp
-----------------------------------------------------------------------------*/

M140::M140(char * command)
{
	s_value =(int) get_value_from_char_array(Printer::instance().command, 'S');

	if(s_value == 0)
	{
		Serial.println("No argument provided for M140, setting temp to 0...");
	}

}

// Will set temp to 0 if argument not found, I consider this good.

void M140::process()
{
	#ifdef DEBUG_GCODE_PROCESSING
		Serial.print("Setting bed temperature to: ");
		Serial.print(s_value);
		Serial.println(" Degrees C");
	#endif
	#ifdef HAS_BED
		Printer::instance().Bed.setTemperature(s_value);
	#endif

}


