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


enum Thermometer
{
THERMISTOR, AD595, MAX6675
};

#include "String.h"
#include "Arduino.h"
#include "AEONS_Config.h"
#include <stdlib.h> 

// Fatal error handler.
#define ASSERT(__value__) 											\
if (!__value__)														\
{																	\
	Serial.print("Fatal Error!\n Check the line in firmware: ");						\
	Serial.println(__LINE__);										\
	while(true){}													\
}

//MACRO AREA
#if HEATER_0_PIN > -1
	#define HAS_EXTRUDER
#endif

#if TEMP_1_PIN > -1
	#define HAS_BED
#endif

#if FAN_PIN > -1
	#define HAS_FAN
#endif

#if PS_ON_PIN > -1
	#define HAS_POWER_SUPPLY
#endif

// void * operator new(size_t size)
// {
  // return malloc(size);
// }

// void operator delete(void * ptr)
// {
  // free(ptr);
// } 
//variable area

int current_x_position = 0;
int current_y_position = 0;
int current_z_position = 0;
int current_e_position = 0;
int current_f_position = 0;

//last line number
long line_number = 0;

typedef int TempInDegrees;
typedef int RawAdcValue;
typedef int Pin;

/*-----------------------------------------------------------------------------
	Implementation of smething that has an active heating device,
	typically an extruder, heated build plate, etc.
	
	Has services for setting, getting, and managing the temperature.
-----------------------------------------------------------------------------*/
class Heater
{
	Thermometer  		_device_type;
	TempInDegrees 		_target;
	TempInDegrees 		_current;
	const TempTableElement*   _temptable;
	Pin                	_sensor_pin; 
	Pin                	_power_pin;

public:
	
	// Ctor.
	Heater(Thermometer deviceType, Pin sensorPin, Pin powerPin, const TempTableElement* givenTable)
	{
		_device_type = deviceType;
		_target = 0;
		_current = 0;
		_temptable = givenTable;
		_sensor_pin = sensorPin;
		_power_pin = powerPin;
		pinMode(_sensor_pin, INPUT);
		pinMode(_power_pin, OUTPUT);
	}

	// Return current temperature.
	TempInDegrees getTemperature()
	{
		update_local_temperature();
		return _current;
	}
	
	void setTemperature(TempInDegrees target)
	{
		_target = target;
		manage_temperature();
	}
	
	// Read thermistor and update class variables.
	void update_local_temperature()
	{
		if (_device_type == THERMISTOR)
		{
			RawAdcValue current_raw = 1023 - analogRead(_sensor_pin);
			for (int i=2; i<=_temptable[0].rawAdcValue; i++)
			{
				// Skip until we find a raw value greater than current_raw.
				if (_temptable[i].rawAdcValue > current_raw)
				{
					// Found one.  Interpolate between this and previous table entry.
					RawAdcValue    fractionalRaw = current_raw - _temptable[i-1].rawAdcValue;
					TempInDegrees  stepTemp = _temptable[i].tempInDegrees - _temptable[i-1].tempInDegrees;
					RawAdcValue    stepRaw = _temptable[i].rawAdcValue - _temptable[i-1].rawAdcValue;
					_current  = _temptable[i-1].tempInDegrees + fractionalRaw * stepTemp / stepRaw;

					break;
				}
			}
		}
		
		if (_device_type == AD595)
		{
			RawAdcValue current_raw = analogRead(_sensor_pin);
			_current = current_raw * 500 / 1024;
		}
	}
	
	void manage_temperature()
	{
		update_local_temperature();
		
		//check if we're at a lower temperature than the target
		if(_current + TEMPDELTA < _target)
			digitalWrite(_power_pin, HIGH);
			
		//check if we're at a higher temperature than the target	
		if(_current - TEMPDELTA > _target)
			digitalWrite(_power_pin, LOW);
			
		//else we must be within the allowed delta of the target
	
	}
	
};



/*-----------------------------------------------------------------------------
	Manages simple I/O device that turns on and off.
-----------------------------------------------------------------------------*/
struct Device
{
	Pin _on_pin;
	boolean _inverting;
	
	//constructor
	Device(Pin onPin, boolean inverting)
	{
		_inverting = inverting;
		_on_pin = onPin;
		pinMode(_on_pin, OUTPUT);
	}
	
	void turn_on()
	{
		digitalWrite(_on_pin, _inverting ? LOW : HIGH);
	}
	
	void turn_off()
	{
		digitalWrite(_on_pin, _inverting ? HIGH : LOW);
	}
	
	
	
};

	#ifdef HAS_POWER_SUPPLY
		Device Power_Supply(PS_ON_PIN, true);
	#endif

	#ifdef HAS_EXTRUDER
		Heater Extruder(extruder_heater_device, TEMP_0_PIN, HEATER_0_PIN, TEMPTABLE);
	#endif
	
	#ifdef HAS_BED
		Heater Bed(extruder_heater_device, TEMP_1_PIN, HEATER_1_PIN, BEDTEMPTABLE);
	#endif

	#ifdef HAS_FAN
		Device Fan(FAN_PIN, false);
	#endif

/*

/*-----------------------------------------------------------------------------
-----------------------------------------------------------------------------*/
struct code
{
	int _n_value;
	char * command;
	//checks that the n-code is equal to the last n-code + 1
	virtual void process() = 0;

};

code * gcode_factory();

/*-----------------------------------------------------------------------------
-----------------------------------------------------------------------------

struct G1 :: code
{
	boolean has_x_value;
	double x_value;
	
	boolean has_y_value;
	double y_value;
	
	boolean has_z_value;
	double z_value;
	
	boolean has_e_value;
	double e_value;
	
	boolean has_f_value;
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


/*-----------------------------------------------------------------------------
M104 (S230) Set extruder temperature to given temp
-----------------------------------------------------------------------------*/
struct M104 : code
{	
	int s_value;
	
	M104(char * command)
	{
		if((s_value =(int) get_value_from_char_array(command, 'S')) = 0)
		{
			Serial.println("No argument provided for M104, setting temp to 0...");
		}
		
	}
	
	// Will set temp to 0 if argument not found, I consider this good.
	
	void process()
	{
		#ifdef HAS_EXTRUDER
			Extruder.setTemperature(s_value);
		#endif
	}

};

/*-----------------------------------------------------------------------------
M80 Turn On Power Supply
-----------------------------------------------------------------------------*/
struct M80 : code
{	

	M80(char * command)
	{
		
	}
	
	void process()
	{
		#ifdef HAS_POWER_SUPPLY
			Power_Supply.turn_on();
		#endif
	}

};
/*-----------------------------------------------------------------------------
-----------------------------------------------------------------------------*/

void setup()
{
    // setup serial connection
	Serial.begin(BAUDRATE);
	Serial.println("start");
	
	//create and init device objects
	//which set up their own pins
	#ifdef HAS_POWER_SUPPLY
		Power_Supply.turn_on();
	#endif
	
	#ifdef HAS_EXTRUDER
		Extruder.setTemperature(DEFAULT_EXTRUDER_TEMP);
	#endif
	
	#ifdef HAS_BED
		Bed.setTemperature(DEFAULT_BED_TEMP);
	#endif

	#ifdef HAS_FAN
		Fan.turn_on();
	#endif

	//initialize pins
	init_pins();
}

/*-----------------------------------------------------------------------------
-----------------------------------------------------------------------------*/
void manage_temperatures()
{
	#ifdef HAS_EXTUDER
		Extruder.manage_temperature();
	#endif
	
	#ifdef HAS_BED
		Bed.manage_temperature();
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
-----------------------------------------------------------------------------*/
code * gcode_factory()
{
	char command[MAX_GCODE_LENGTH];
	
	// Get next GCode command.
	get_next_command(command, sizeof(command));
	
	fix_comments(command);
	
	#ifdef DEBUG_GCODE_PROCESSING
		Serial.printf("Complete read gcode: %s", command);
	#endif
	
	//next part sets variables to attributes of the recieved code
	boolean has_g_value = false;
	int g_value;
	if(get_value_from_char_array(command, 'G') != 0.0)
	{
		has_g_value = true;
		g_value =(int) get_value_from_char_array(command, 'G');
	}
	
	boolean has_m_value;
	int m_value = 0;
	if(get_value_from_char_array(command, 'M') != 0.0)
	{
		has_m_value = true;
		m_value = (int)get_value_from_char_array(command, 'M');
	}
	
	boolean has_n_value;
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
			case 104:
				return new M104(command);
			
			case 80:
				//return new M80(command);
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