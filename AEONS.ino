/*----------------------------------__-----------------------------------------
   /\     |----     __     |\   |  /  \
  /  \    |____   /    \   | \  |  \___
 /----\   |      |      |  |  \ |      \
/      \  |----   \ __ /   |   \|   ___/
                                       ...because C++ isn't quite so bad after all.


	Implemented Codes
	-------------------
	Gcode:

	Official RepRap M-Codes:
	M104 - Set extruder target temp

	Custom M-Codes:
	M80  - Turn on Power Supply

	To Be Implemented
	-------------------
	 G0  -> G1
	 G1  - Coordinated Movement X Y Z E
	 G4  - Dwell S<seconds> or P<milliseconds>
	 G28 - Home all Axis
	 G90 - Use Absolute Coordinates
	 G91 - Use Relative Coordinates
	 G92 - Set current position to cordinates given

	RepRap M Codes
	 M105 - Read current temp
	 M106 - Fan on
	 M107 - Fan off
	 M109 - Wait for extruder current temp to reach target temp.
	 M114 - Display current position

	Custom M Codes
	 M42 - Set output on free pins, on a non pwm pin (over pin 13 on an arduino mega) use S255 to turn it on and S0 to turn it off. Use P to decide the pin (M42 P23 S255) would turn pin 23 on
	 M81  - Turn off Power Supply
	 M82  - Set E codes absolute (default)
	 M83  - Set E codes relative while in Absolute Coordinates (G90) mode
	 M84  - Disable steppers until next move, 
			or use S<seconds> to specify an inactivity timeout, after which the steppers will be disabled.  S0 to disable the timeout.
	 M85  - Set inactivity shutdown timer with parameter S<seconds>. To disable set zero (default)
	 M92  - Set axis_steps_per_unit - same syntax as G92
	 M115	- Capabilities string
	 M140 - Set bed target temp
	 M190 - Wait for bed current temp to reach target temp.
	 M201 - Set max acceleration in units/s^2 for print moves (M201 X1000 Y1000)
	 M202 - Set max acceleration in units/s^2 for travel moves (M202 X1000 Y1000)

	 
	 By MultipleMonomials and ChatterComa, thx to Kliment
-----------------------------------------------------------------------------*/

#include "AEONS.h"
#include "String.h"
#include "Arduino.h"
#include <stdlib.h> 
#include "Printer.h"
#include "gcodes.h"
#include "assert.h"


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

#if 0

struct G1 : code
{
	bool has_x_value;
	double x_value;
	
	bool has_y_value;
	double y_value;
	
	bool has_z_value;
	double z_value;
	
	bool has_e_value;
	double e_value;
	
	bool has_f_value;
	double f_value;
	
	G1(char * command, int n_value)
	{
		has_x_value = false;
		if((x_value = get_value_from_char_array(command, 'X')) != 0.0)
		{
			has_x_value = true;
		}
		
		has_y_value = false;
		if((y_value = get_value_from_char_array(command, 'Y')) != 0.0)
		{
			has_y_value = true;
		}
		
		has_z_value = false;
		if((z_value = get_value_from_char_array(command, 'Z')) != 0.0)
		{
			has_z_value = true;
		}
		
		has_e_value = false;
		if((e_value = get_value_from_char_array(command, 'E')) != 0.0)
		{
			has_e_value = true;
		}
		
		has_f_value = false;
		if((f_value = get_value_from_char_array(command, 'F')) != 0.0)
		{
			has_f_value = true;
		}
	}
};
#endif

/*-----------------------------------------------------------------------------
-----------------------------------------------------------------------------*/

void setup()
{
    // setup serial connection
	Serial.begin(BAUDRATE);
	Serial.println("start");
	
	//create and init device objects
	//which set up their own pins

	//initialize pins
	init_pins();
}

/*-----------------------------------------------------------------------------
-----------------------------------------------------------------------------*/
void manage_temperatures()
{
	#ifdef HAS_EXTUDER
		Printer::instance().Extruder.manage_temperature();
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
		code * code_recieved = gcode_factory();
		if(code_recieved!=NULL)
		{
		//call the code object's process function and then free it from memory
			code_recieved->process();
			delete(code_recieved);
		}
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
	int counter = 0;
	do
	{
		//If you're getting assert failed, increase the MAX_GCODE_SIZE variable in the config file
		ASSERT(counter <= buffer_length);
		buffer[counter] = Serial.read();
	} 
	while (buffer[counter++] != '\n');
	
	
	// Convert to null-terminated string.
	buffer[counter-1] = '\0';
}

/*-----------------------------------------------------------------------------
-----------------------------------------------------------------------------*/
void validate(int n_value)
{
	if(n_value != line_number + 1)
	{
		Serial.print("ERROR: The supplied line number of ");
		Serial.println(n_value);
		Serial.print(" is not eqaual to ");
		Serial.print(line_number);
		Serial.println(" + 1.");
		return;
	}
	
	else
	{
		//update the curent line number
		line_number = n_value;
		return;
	}
}

/*-----------------------------------------------------------------------------
Reads one command from the serial port and returns a code object of the correct type
-----------------------------------------------------------------------------*/
code * gcode_factory()
{
	char command[MAX_GCODE_LENGTH];
	
	// Get next GCode command.
	get_next_command(command, sizeof(command));
	
	fix_comments(command);
	
	#ifdef DEBUG_GCODE_PROCESSING
		Serial.print("Complete read gcode: ");
		Serial.println(command);
	#endif
	
	//next part sets variables to attributes of the recieved code
	bool has_g_value = false;
	int g_value;
	if(get_value_from_char_array(command, 'G') != 0.0)
	{
		has_g_value = true;
		g_value =(int) get_value_from_char_array(command, 'G');
	}
	
	bool has_m_value;
	int m_value = 0;
	if(get_value_from_char_array(command, 'M') != 0.0)
	{
		has_m_value = true;
		m_value = (int)get_value_from_char_array(command, 'M');
	}
	
	bool has_n_value;
	int n_value = 0;
	if(get_value_from_char_array(command, 'M') != 0.0)
	{
		has_n_value = true;
		n_value = (int) get_value_from_char_array(command, 'N');
	}
	
	if(!(has_g_value && has_m_value))
		Serial.println("Error! Neither a g-value or an m-value were recieved!");
	
	
	validate(n_value);
	
	//Now we construct the correct Gcode object
	if(has_g_value)
	{
		//cast here should be OK since mcodes and gcodes are never decimals
		switch((int)g_value)
		{
			case 0:
			case 1:
				break;
		}
	}
	
	else if(has_m_value)
	{
		switch((int) m_value)
		{
			case 80:
				return new M80(command);
				break;

			case 104:
				return new M104(command);
				break;
				
			case 105:
				return new M105(command);
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

	if (pointer_to_target == NULL)
	{
		//check if target is in code
		#ifdef DEBUG_GCODE_PROCESSING
			Serial.print("get_value_from_char_array: could not find target char: ");
			Serial.println(target);
			Serial.println("This occurs under normal operation, but if you are having problems this could be the cause");
		#endif
		return 0;
	}
	
	char * end_ptr; // strtod returns a double (and a char*) from a char*
	double code_value = strtod((pointer_to_target + 1), &end_ptr);
	
	return code_value;
}

/*-----------------------------------------------------------------------------
-----------------------------------------------------------------------------*/
void fix_comments(char * command)
{
	// Find comment delimiter if any.
	char * semicolon_ptr = 	strchr(command, ';');
	
	// Truncate command at semicolon.
	if (semicolon_ptr != NULL)
	{
		*semicolon_ptr = '\0';
	}
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
  
  //Initialize Enable Pins - steppers default to disabled.
  
  #if (X_ENABLE_PIN > -1)
    pinMode(X_ENABLE_PIN, OUTPUT);
  if(!X_ENABLE_ON) digitalWrite(X_ENABLE_PIN,HIGH);
  #endif
  #if (Y_ENABLE_PIN > -1)
    pinMode(Y_ENABLE_PIN, OUTPUT);
  if(!Y_ENABLE_ON) digitalWrite(Y_ENABLE_PIN,HIGH);
  #endif
  #if (Z_ENABLE_PIN > -1)
    pinMode(Z_ENABLE_PIN, OUTPUT);
  if(!Z_ENABLE_ON) digitalWrite(Z_ENABLE_PIN,HIGH);
  #endif
  #if (E_ENABLE_PIN > -1)
    pinMode(E_ENABLE_PIN, OUTPUT);
  if(!E_ENABLE_ON) digitalWrite(E_ENABLE_PIN,HIGH);
  #endif
  
  //endstops and pullups
  #ifdef ENDSTOPPULLUPS
  #if X_MIN_PIN > -1
    pinMode(X_MIN_PIN, INPUT); 
    digitalWrite(X_MIN_PIN,HIGH);
  #endif
  #if X_MAX_PIN > -1
    pinMode(X_MAX_PIN, INPUT); 
    digitalWrite(X_MAX_PIN,HIGH);
  #endif
  #if Y_MIN_PIN > -1
    pinMode(Y_MIN_PIN, INPUT); 
    digitalWrite(Y_MIN_PIN,HIGH);
  #endif
  #if Y_MAX_PIN > -1
    pinMode(Y_MAX_PIN, INPUT); 
    digitalWrite(Y_MAX_PIN,HIGH);
  #endif
  #if Z_MIN_PIN > -1
    pinMode(Z_MIN_PIN, INPUT); 
    digitalWrite(Z_MIN_PIN,HIGH);
  #endif
  #if Z_MAX_PIN > -1
    pinMode(Z_MAX_PIN, INPUT); 
    digitalWrite(Z_MAX_PIN,HIGH);
  #endif
  #else
  #if X_MIN_PIN > -1
    pinMode(X_MIN_PIN, INPUT); 
  #endif
  #if X_MAX_PIN > -1
    pinMode(X_MAX_PIN); 
  #endif
  #if Y_MIN_PIN > -1
    pinMode(Y_MIN_PIN, INPUT); 
  #endif
  #if Y_MAX_PIN > -1
    pinMode(Y_MAX_PIN, INPUT); 
  #endif
  #if Z_MIN_PIN > -1
    pinMode(Z_MIN_PIN, INPUT); 
  #endif
  #if Z_MAX_PIN > -1
    pinMode(Z_MAX_PIN, INPUT); 
  #endif
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