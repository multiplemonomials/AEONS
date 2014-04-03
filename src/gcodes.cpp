/*-----------------------------------------------------------------------------
	gcodes.cpp
	Code file fr all of the gcode objects
	By MultipleMonomials and ChatterComa
-----------------------------------------------------------------------------*/

#include "gcodes.h"


/*-----------------------------------------------------------------------------
-----------------------------------------------------------------------------*/

G1::G1(char * command)
{
	//with a G1 command, if an axis is not included in the command, that axis shouldn't move.
	//if we're in relative mode, that axis should move 0.
	//if we're in absolute mode, that axis should move to is current position
	//the trouble is that, in absolute mode, "G1 X0 Y10" is different from "G1 Y10"

	x_value = find_double_in_command(command, 'X');
	y_value = find_double_in_command(command, 'Y');
	z_value = find_double_in_command(command, 'Z');
	e_value = find_double_in_command(command, 'E');
	f_value = find_double_in_command(command, 'F');
	if(!Printer::instance().relative_mode)
	{
		if(test_for_char(command, 'X'))
		{
			x_value = Printer::instance().x_axis.getCurrentPosition();
		}

		if(test_for_char(command, 'Y'))
		{
			y_value = Printer::instance().y_axis.getCurrentPosition();
		}

		if(test_for_char(command, 'Z'))
		{
			z_value = Printer::instance().z_axis.getCurrentPosition();
		}

		if(test_for_char(command, 'E'))
		{
			e_value = Printer::instance().e_axis.getCurrentPosition();
		}
	}

	if(f_value == 0)
	{
		//an f value was not included in the command (or was 0), so use the last provided one
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


	Movement movement(x_value, y_value, z_value, e_value, f_value);
	if(movement.is_valid())
	{
		movement.calculate_values();
		movement.execute();
	}
	else
	{
		#ifdef DEBUG_MOVEMENT
			Serial.println("ERROR: Movement is not valid (no change in position)");
		#endif
	}
}

/*-----------------------------------------------------------------------------
G4 Delay Pxxx Milliseconds
-----------------------------------------------------------------------------*/
G4::G4(char * command)
{
	_command = command;
	p_value = (int)find_double_in_command(command, 'P');
}

/*-----------------------------------------------------------------------------

-----------------------------------------------------------------------------*/
void G4::process()
{
	delay(p_value);
}


/*-----------------------------------------------------------------------------
G28 Home All
-----------------------------------------------------------------------------*/
G28::G28(char * command)
{
	_command =  command;
}

/*-----------------------------------------------------------------------------

-----------------------------------------------------------------------------*/
void G28::process()
{
	// If no argument supplied, home all axes.
	if((!test_for_char(_command, 'X')) && (!test_for_char(_command, 'Y')) && (!test_for_char(_command, 'Z')))
	{
		#ifdef DEBUG_GCODE_PROCESSING
			Serial.println("Homing X...");
		#endif
		Printer::instance().x_axis.home();
		#ifdef DEBUG_GCODE_PROCESSING
			Serial.println("Homing Y...");
		#endif
		Printer::instance().y_axis.home();
		Printer::instance().z_axis.home();
		return;
	}

	// Home individual axes as requested.

	if(test_for_char(_command, 'X'))
	{
		Printer::instance().x_axis.home();
	}

	if(test_for_char(_command, 'Y'))
	{
		Printer::instance().y_axis.home();
	}

	if(test_for_char(_command, 'Z'))
	{
		Printer::instance().z_axis.home();
	}
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
G92 Set position
-----------------------------------------------------------------------------*/
G92::G92(char * command)
{
	//read arguments from command
	has_x_value = test_for_char(command, 'X');
	has_y_value = test_for_char(command, 'Y');
	has_z_value = test_for_char(command, 'Z');
	has_e_value = test_for_char(command, 'E');

	if(has_x_value)
	{
		x_value = find_double_in_command(command, 'X');
	}
	if(has_y_value)
	{
		y_value = find_double_in_command(command, 'Y');
	}
	if(has_z_value)
	{
		z_value = find_double_in_command(command, 'Z');
	}
	if(has_e_value)
	{
		e_value = find_double_in_command(command, 'E');
	}
}

void G92::process()
{
	if(has_x_value)
	{
		Printer::instance().x_axis.setCurrentPosition(x_value);
	}

	if(has_y_value)
	{
		Printer::instance().y_axis.setCurrentPosition(y_value);
	}

	if(has_z_value)
	{
		Printer::instance().z_axis.setCurrentPosition(z_value);
	}

	if(has_e_value)
	{
		Printer::instance().e_axis.setCurrentPosition(e_value);
	}

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

				clear_command(Printer::instance().command);
				delete code_to_process;
		}

		//reached end of command
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
			Serial.println("Recieved M81, turning off power supply...");
	#endif
}

/*-----------------------------------------------------------------------------
M84 Disable Motors
-----------------------------------------------------------------------------*/

M84::M84(char * command)
{
	_wait_time = find_double_in_command(command, 'S');
}

void M84::process()
{
	Printer::instance().x_axis.disable();
	Printer::instance().y_axis.disable();
	Printer::instance().z_axis.disable();
	Printer::instance().e_axis.disable();
}

/*-----------------------------------------------------------------------------
M92 Set Steps Per MM
-----------------------------------------------------------------------------*/

M92::M92(char * command)
{
	x_value = find_double_in_command(command, 'X');
	y_value = find_double_in_command(command, 'Y');
	z_value = find_double_in_command(command, 'Z');
	e_value = find_double_in_command(command, 'E');
}

void M92::process()
{
	Printer::instance().x_axis._steps_per_mm = x_value;
	Printer::instance().y_axis._steps_per_mm = y_value;
	Printer::instance().z_axis._steps_per_mm = z_value;
	Printer::instance().e_axis._steps_per_mm = e_value;
}

/*-----------------------------------------------------------------------------
M104 (Sxxx) Set extruder temperature to given temp
If you have multiple extruders, it will set the temp for ONLY the current one!
-----------------------------------------------------------------------------*/

M104::M104(char * command)
{
	s_value =(int) find_double_in_command(command, 'S');

	#ifdef DEBUG_GCODE_PROCESSING
		if(s_value == 0)
		{
			Serial.println("No argument (M104 S<temp goes here>) provided for M104, setting temp to 0...");
		}
	#endif
	
}

// Will set temp to 0 if argument not found, I consider this desirable.

void M104::process()
{
	#ifdef M104_AFFECTS_ALL_EXTRUDERS
		Printer::instance().Extruder_1.setTemperature(s_value);
		Printer::instance().Extruder_2.setTemperature(s_value);
	#else
		#ifdef HAS_EXTRUDER
			Printer::instance().Extruder.setTemperature(s_value);
		#endif
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
	Serial.print("ok\n");
	#ifdef HAS_EXTRUDER
		Serial.print("T:");
		Serial.print(getCurrentExtruderTemperature());
	#endif
	#ifdef HAS_BED
		Serial.print(" B:");
		Serial.print(Printer::instance().Bed.getTemperature());
	#endif

		Serial.print('\n');

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

/*-----------------------------------------------------------------------------
M107 Wait for Extruder Tempurature
-----------------------------------------------------------------------------*/

M109::M109(char * command)
{
	s_value =(int) find_double_in_command(command, 'S');
}

void M109::process()
{
	#ifdef HAS_EXTRUDER
		Printer::instance().Extruder.setTemperature(s_value);
		//while(Printer::instance().Extruder.getTemperature() )
	#endif
}

/*-----------------------------------------------------------------------------
M114 - Report assumed postion to host
-----------------------------------------------------------------------------*/

M114::M114(char * command)
{

}

//Where am I?
void M114::process()
{
	Serial.println("C: ");
	Serial.print("X: ");
	Serial.println(Printer::instance().x_axis._current_position);
	Serial.print("Y: ");
	Serial.println(Printer::instance().y_axis._current_position);
	Serial.print("Z: ");
	Serial.println(Printer::instance().z_axis._current_position);
	Serial.print("E: ");
	Serial.println(Printer::instance().e_axis._current_position);
}

/*------------------------------------------------------------------------------------
M116 [Ssss] [Pppp]-- Wait for extruder and bed to heat up (can specify temps for both)
------------------------------------------------------------------------------------*/

M116::M116(char * command)
{
	//user can optionally set temp values
	s_value =(int) find_double_in_command(command, 'S');
	p_value =(int) find_double_in_command(command, 'P');


}

void M116::process()
{
	bool extruder_up_to_temp =
	#ifdef HAS_EXTRUDER
		false;
	#else
		true;
	#endif

	bool extruder_2_up_to_temp =
	#ifdef HAS_SECOND_EXTRUDER
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

		#ifdef HAS_EXTRUDER
			extruder_up_to_temp = //not too cold
					(Printer::instance().Extruder.getTemperature() >= (Printer::instance().Extruder.getTarget() - TEMPDELTA)) &&
					//not too hot
					(Printer::instance().Extruder.getTemperature() <= (Printer::instance().Extruder.getTarget() + TEMPDELTA));
		#endif


		Serial.print("ok T:");
		Serial.print(getCurrentExtruderTemperature());


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
	while(!((extruder_up_to_temp && extruder_2_up_to_temp)&& bed_up_to_temp));

}

/*-----------------------------------------------------------------------------
M140 <Ssss>--Set bed temperature to given temp
-----------------------------------------------------------------------------*/

M140::M140(char * command)
{
	s_value =(int) find_double_in_command(Printer::instance().command, 'S');

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

/*-----------------------------------------------------------------------------
M201 - Set max [printing] acceleration
-----------------------------------------------------------------------------*/
M201::M201(char * command)
{
	//keep searching the possible arguments until we find one that is not zero

	acceleration_value = find_long_in_command(Printer::instance().command, 'X');
	if(acceleration_value != 0)
	{
		return;
	}

	acceleration_value = find_long_in_command(Printer::instance().command, 'Y');
	if(acceleration_value != 0)
	{
		return;
	}

	acceleration_value = find_long_in_command(Printer::instance().command, 'Z');
	if(acceleration_value != 0)
	{
		return;
	}

	acceleration_value = find_long_in_command(Printer::instance().command, 'E');
	if(acceleration_value != 0)
	{
		return;
	}


}

void M201::process()
{
	if((acceleration_value == 0) || (acceleration_value == 1))
	{
		//we didn't get anything
		//1 is the kryptonite of our acceleration algorithm, as 1-(1/1) = 0
		return;
	}

	Printer::instance().max_acceleration = acceleration_value;

}

