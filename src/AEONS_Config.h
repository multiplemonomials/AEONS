#ifndef CONFIGURATION_H
#define CONFIGURATION_H

// BASIC SETTINGS: select your board type, thermistor type, axis scaling, and endstop configuration

//// The following define selects which electronics board you have. Please choose the one that matches your setup
// Gen 3 Plus = 21
// gen 3  Monolithic Electronics = 22
// Gen3 PLUS for TechZone Gen3 Remix Motherboard = 23
// RAMPS 1.0 = 30
// RAMPS 1.1/1.2  = 31
// RAMPS 1.3/1.4 = 33
// Gen6 = 5
// Gen7 1.2 = 72
// Gen7 1.3 - 1.4 = 73
// Sanguinololu up to 1.1 = 6
// Sanguinololu 1.2 and above = 62
// Teensylu (at90usb) = 8

#define MOTHERBOARD 33

#include "pins.h"
#include "Thermometer.h"

 //software endstop settings:
 //Endstop pins are set to your board defaults.  If your endstop situation is different, you can change them here.
 //Set the pin to -1 if you don't have an endstop. 
 //#define X_MIN_PIN           3
 //#define X_MAX_PIN          -1
 
 //	#define Y_MIN_PIN          14
 //	#define Y_MAX_PIN          -1
 
 // #define Z_MIN_PIN          18
 // #define Z_MAX_PIN          -1
	
//// Thermistor settings:
// 0 is your own custom table (YOU MUST ENTER IN thermistortables.h!!!)
// 1 is 100k thermistor
// 2 is 200k thermistor
// 3 is mendel-parts thermistor
// 4 is 10k thermistor
// 5 is ParCan supplied 104GT-2 100K
// 6 is EPCOS 100k
// 7 is 100k Honeywell thermistor 135-104LAG-J01
#define THERMISTORHEATER 1
#define THERMISTORBED 1

//// Endstop Settings
#define ENDSTOPPULLUPS // Comment this out (using // at the start of the line) to disable the endstop pullup resistors
// The pullups are needed if you directly connect a mechanical endswitch between the signal and ground pins.
//If your axes are only moving in one direction, make sure the endstops are connected properly.


// This determines the communication speed of the printer
#define BAUDRATE 115200

//// ADVANCED SETTINGS - to tweak thermistor temps:
#include "thermistortables.h"

// For Inverting Stepper Enable Pins (Active Low) use 0 (e.g.polulu), Non Inverting (Active High) use 1
#define X_ENABLE_ON 0
#define Y_ENABLE_ON 0
#define Z_ENABLE_ON 0
#define E_ENABLE_ON 0

//// ENDSTOP SETTINGS:
// Sets direction of endstops when homing; 1=MAX, -1=MIN
#define X_HOME_DIR -1
#define Y_HOME_DIR -1
#define Z_HOME_DIR -1


// The minimal temperature defines the temperature below which the heater will not be enabled.
// Protects from open circuits.
#define MINTEMP 5

//// Experimental max temp
// When temperature exceeds max temp, your heater will be switched off.
// This feature exists to protect your hotend from overheating accidentally, but *NOT* from thermistor short/failure!
#define MAXTEMP 260

//temperature delta to prevent superfast heater oscilation
#define TEMPDELTA 5

// Lengh of gcode storage array
#define MAX_GCODE_LENGTH 50

//Default temperature to start heating up to when the firmware starts
//USE WITH CAUTION
#define DEFAULT_EXTRUDER_TEMP  0
#define DEFAULT_BED_TEMP 0


//This is for controlling a fan to cool down the stepper drivers
//set it to -1 to disable the fan
//#define FAN_PIN -1
//Uncomment one of the following lines to indicate hot your fan is controlled
//#define FAN_USES_PWM //PWM fan, from a PC
//#define FAN_ON_OFF //Fan turned on by recieving power.

//You can control debugging below the following line:
//#define DEBUG_RAW_TEMP_VALUE //for creating your own thermistor table
#define DEBUG_GCODE_PROCESSING //are the sent gcodes being interpreted properly? SEVERE LAG MAY ENSUE
//#define DEBUG_GCODE_PARSING  //debug serial reading and parsing.

#if HEATER_0_PIN > -1
	#define HAS_EXTRUDER
#endif

#if TEMP_1_PIN > -1
	#define HAS_BED
#endif

#if (FAN_PIN > -1)
	#define HAS_FAN
#endif

#if PS_ON_PIN > -1
	#define HAS_POWER_SUPPLY
#endif

	//If your axes move in one direction ONLY when the endstops are triggered, set [XYZ]_ENDSTOP_INVERT to true here:
	#define X_ENDSTOP_INVERT true
	#define Y_ENDSTOP_INVERT true
	#define Z_ENDSTOP_INVERT true
	
	// Disables axis when it's not being used.
	#define DISABLE_X false
	#define DISABLE_Y false
	#define DISABLE_Z false
	#define DISABLE_E false

	// Inverting axis direction
	#define INVERT_X_DIR false
	#define INVERT_Y_DIR false
	#define INVERT_Z_DIR true
	#define INVERT_E_DIR false
	
	#define MIN_SOFTWARE_ENDSTOPS false //If true, axis won't move to coordinates less than zero.
	#define MAX_SOFTWARE_ENDSTOPS true  //If true, axis won't move to coordinates greater than the defined lengths below.
	#define X_MAX_LENGTH 186
	#define Y_MAX_LENGTH 231
	#define Z_MAX_LENGTH 110

//			NUMERICAL/MOVEMENT SETTINGS HAVE MOVED TO PRINTER.CPP!

#endif

