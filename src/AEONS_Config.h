#ifndef CONFIGURATION_H
#define CONFIGURATION_H

// BASIC SETTINGS: select your board type, thermistor type, and endstop type

// The following selects which electronics board you have. Please choose the one that matches your setup
// custom pin assignment in pins.h = 0
// RepRap Motherboard = 1
// Gen3 = not supported, sorry
// RAMPS = 3
// Gen6 = 5
// Gen7 1.2 = 7
// Sanguinololu = 6
// Teensylu (at90usb) = 8

#define MOTHERBOARD_MODEL 3

// Now set your motherboard version:

//0                                 |RAMPS 1.0, Gen6 1.0, teensylu, Reprap Motherboard
//1									|RAMPS 1.1, Sanguinolulu 1.1, Gen3
//2									|RAMPS 1.2, Sanguinolulu 1.2, Gen7 1.2
//3									|RAMPS 1.3, Gen7 1.3
//4									|RAMPS 1.4, Gen7 1.4


#define MOTHERBOARD_VERSION 3

#include "pins.h"
#include "Thermometer.h"

 //software endstop settings:

//#define MIN_SOFTWARE_ENDSTOPS //If uncommented, axis won't move to coordinates less than zero.
#define MAX_SOFTWARE_ENDSTOPS //If true, axis won't move to coordinates greater than the defined lengths below.

//#define MAX_HARDWARE_ENDSTOPS //your endstops are mounted at the end of your axes
#define MIN_HARDWARE_ENDSTOPS //your endstops are mounted at the start of your axes
//using both at the same time is not supported


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

// This determines the communication speed of the printer
#define BAUDRATE 115200

//// ADVANCED SETTINGS - to tweak thermistor temps:
#include "thermistortables.h"

//Temperature settings
//-------------------------------------------------------------------------------------------------------------------------------------------

// When temperature exceeds max temp, your heater will be switched off.
// This feature exists to protect your hotend from overheating accidentally, but *NOT* from thermistor failure!
#define MAXTEMP 260

// The minimal temperature defines the temperature below which the heater will not be enabled.
// Protects from open circuits.
#define MINTEMP 5

//temperature delta to prevent superfast heater oscilation
#define TEMPDELTA 2

// Lengh of gcode storage array
#define MAX_GCODE_LENGTH 50

//Default temperature to start heating up to when the firmware starts
//USE WITH CAUTION
#define DEFAULT_EXTRUDER_TEMP  0
#define DEFAULT_BED_TEMP 0


//This is for controlling a fan to cool down the stepper drivers
//set it to -1 to disable the fan
//#define FAN_PIN -1
//Uncomment one of the following lines to indicate how your fan is controlled
//#define FAN_USES_PWM //PWM fan.  CURRENTLY NOT SUPPORTED
#define FAN_ON_OFF //Fan turned on by recieving power.

//Debugging Settings:
//-------------------------------------------------------------------------------------------------------------------------------------------

//You can control debugging below the following line:
// Comment these out (using // at the start of the line)
//#define DEBUG_RAW_TEMP_VALUE //for creating your own thermistor table
#define DEBUG_GCODE_PROCESSING //are the sent gcodes being interpreted properly? SEVERE LAG MAY (or may not) ENSUE
//#define DEBUG_GCODE_PARSING  //debug serial reading and parsing.
#define DEBUG_MOVEMENT       //debug movement calculation
#define RUN_UNIT_TESTS  // Uncomment to run unit tests at startup.
#define ACTUALLY_MOVE   //does the printer move when told to?
//#define DEBUG_ENDSTOPS  //why won't that axis move?

//MOVEMENT SETTINGS
//-------------------------------------------------------------------------------------------------------------------------------------------

// Inverting axis direction
#define INVERT_X_DIR false
#define INVERT_Y_DIR false
#define INVERT_Z_DIR true
#define INVERT_E_DIR false

// For Inverting Stepper Enable Pins (Active Low) use 1 (e.g. polulu), Non Inverting (Active High) use 0
#define ENABLE_PINS_INVERTING 1

#define X_MAX_LENGTH 186
#define Y_MAX_LENGTH 231
#define Z_MAX_LENGTH 110

#define X_STEPS_PER_MM 52.504
#define Y_STEPS_PER_MM 26.523
#define Z_STEPS_PER_MM 2267.567
#define E_STEPS_PER_MM 760.0

//feedrate limits for axes--defaults should be OK for most people
#define XY_AXES_MAX_FEEDRATE 18000
#define Z_AXIS_MAX_FEEDRATE 100
#define E_AXIS_MAX_FEEDRATE 300

//feedrates to use when sent a G28 Home All Axes
#define XY_AXES_HOMING_FEEDRATE 1500
#define ZE_AXES_HOMING_FEEDRATE 75

//// Endstop Settings
//If your axes are only moving in one direction, make sure the endstops are connected properly.
//If your axes move in one direction ONLY when the endstops are triggered, set [XYZ]_ENDSTOP_INVERT to true here:
#define X_ENDSTOP_INVERT true
#define Y_ENDSTOP_INVERT true
#define Z_ENDSTOP_INVERT true

//optimization settings:
//-------------------------------------------------------------------------------------------------------------------------------------------
//check the endstop state while we're moving, or rely on software endstops?  May increase maximum speed.
//#define ENDSTOP_CHECK_DURING_MOVE

//verify gcodes with a checksum if your host supports it (pronterface does not)
//may give a speed improvement if disabled, leave on if not sure
#define VERIFY_GCODES

//User settings above this line, don't mess with stuff below
//-------------------------------------------------------------------------------------------------------------------------------------------

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

#ifdef MAX_HARDWARE_ENDSTOPS
	#define ENDSTOPS_AT_MIN false
#else
	#define ENDSTOPS_AT_MIN true
#endif

#ifdef MIN_HARDWARE_ENDSTOPS
	#ifdef MAX_HARDWARE_ENDSTOPS
		#error "You can't use minimum and maximum hardware endstops at the same time. Sorry."
	#endif
#endif

#ifdef MIN_HARDWARE_ENDSTOPS
	#define X_ENDSTOP_PIN X_MIN_PIN
#else
	#define X_ENDSTOP_PIN X_MAX_PIN
#endif

#ifdef MIN_HARDWARE_ENDSTOPS
	#define Y_ENDSTOP_PIN Y_MIN_PIN
#else
	#define Y_ENDSTOP_PIN Y_MAX_PIN
#endif

#endif
#ifdef MIN_HARDWARE_ENDSTOPS
	#define Z_ENDSTOP_PIN Z_MIN_PIN
#else
	#define Z_ENDSTOP_PIN Z_MAX_PIN
#endif

//enable min software endstops if ENDSTOP_CHECK_DURING_MOVE is disabled
#ifdef MIN_SOFTWARE_ENDSTOPS
#ifndef MIN_HARDWARE_ENDSTOPS
#ifndef ENDSTOP_CHECK_DURING_MOVE
#define MIN_HARDWARE_ENDSTOPS
#endif
#endif
#endif

//vice versa
#ifdef MAX_SOFTWARE_ENDSTOPS
#ifndef MAX_HARDWARE_ENDSTOPS
#ifndef ENDSTOP_CHECK_DURING_MOVE
#define MAX_HARDWARE_ENDSTOPS
#endif
#endif
#endif
