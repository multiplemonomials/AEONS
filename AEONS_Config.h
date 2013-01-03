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
// Gen7 1.3 = 73
// Sanguinololu up to 1.1 = 6
// Sanguinololu 1.2 and above = 62
// Teensylu (at90usb) = 8

#define MOTHERBOARD 33

#include "pins.h"

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

// Select one of these only to define how the extruder temp is read.
const Thermometer extruder_heater_device = THERMISTOR;
//const Thermometer extruder_heater_device = AD595;
//MAX6675 CURRENTLY NOT SUPPORTED

// Select one of these only to define how the bed temp is read.
const Thermometer bed_heater_device = THERMISTOR;
//const Thermometer bed_heater_device = AD595;

//// Calibration variables
// X, Y, Z, E steps per unit - Metric Prusa Mendel with Wade extruder:
float axis_steps_per_mm[] = {52.504, 26.523, 2267.567, 760.0}; 
// Metric Prusa Mendel with Makergear geared stepper extruder:
//float axis_steps_per_unit[] = {80,80,3200/1.25,1380}; 
// MakerGear Hybrid Prusa Mendel:
// Z axis value is for .9 stepper(if you have 1.8 steppers for Z, you need to use 2272.7272)
//float axis_steps_per_unit[] = {104.987, 104.987, 4545.4544, 1487};

//// Endstop Settings
#define ENDSTOPPULLUPS // Comment this out (using // at the start of the line) to disable the endstop pullup resistors
// The pullups are needed if you directly connect a mechanical endswitch between the signal and ground pins.
//If your axes are only moving in one direction, make sure the endstops are connected properly.
//If your axes move in one direction ONLY when the endstops are triggered, set [XYZ]_ENDSTOP_INVERT to true here:
const bool X_ENDSTOP_INVERT = true;
const bool Y_ENDSTOP_INVERT = true;
const bool Z_ENDSTOP_INVERT = true;

// This determines the communication speed of the printer
#define BAUDRATE 115200

//// ADVANCED SETTINGS - to tweak thermistor temps:
#include "thermistortables.h"

// For Inverting Stepper Enable Pins (Active Low) use 0 (e.g.polulu), Non Inverting (Active High) use 1
#define X_ENABLE_ON 0
#define Y_ENABLE_ON 0
#define Z_ENABLE_ON 0
#define E_ENABLE_ON 0
// Disables axis when it's not being used.
const bool DISABLE_X = false;
const bool DISABLE_Y = false;
const bool DISABLE_Z = false;
const bool DISABLE_E = false;

// Inverting axis direction
const bool INVERT_X_DIR = false;
const bool INVERT_Y_DIR = false;
const bool INVERT_Z_DIR = true;
const bool INVERT_E_DIR = false;

//// ENDSTOP SETTINGS:
// Sets direction of endstops when homing; 1=MAX, -1=MIN
#define X_HOME_DIR -1
#define Y_HOME_DIR -1
#define Z_HOME_DIR -1

const bool min_software_endstops = false; //If true, axis won't move to coordinates less than zero.
const bool max_software_endstops = true;  //If true, axis won't move to coordinates greater than the defined lengths below.
const int X_MAX_LENGTH = 186;
const int Y_MAX_LENGTH = 231;
const int Z_MAX_LENGTH = 110;

//// MOVEMENT SETTINGS
const int NUM_AXIS = 4; // The axis order in all axis related arrays is X, Y, Z, E
float max_feedrate[] = {18000, 18000, 100, 18000};
float homing_feedrate[] = {1500,1500,100};
bool axis_relative_modes[] = {false, false, false, false};

//// The minimal temperature defines the temperature below which the heater will not be enabled
#define MINTEMP 0

//// Experimental max temp
// When temperature exceeds max temp, your heater will be switched off.
// This feature exists to protect your hotend from overheating accidentally, but *NOT* from thermistor short/failure!
// You should use MINTEMP for thermistor short/failure protection.
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

//You can control debugging below the following line:
//#define DEBUG_RAW_TEMP_VALUE //for creating your own thermistor table
//#define DEBUG_GCODE_PROCESSING //are the sent gcodes being interpreted properly? SEVERE LAG MAY ENSUE

 

#endif

