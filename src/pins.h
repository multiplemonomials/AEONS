
#ifndef PINS_H
#define PINS_H
#define ALARM_PIN          -1

/****************************************************************************************
* Custom pin assignment
****************************************************************************************/
#if MOTHERBOARD_MODEL == 0
	#if (!defined(__AVR_ATmega1280__)) && (!defined(__AVR_ATmega2560__))
	 	 #error "Oops!  Make sure you have 'Arduino Mega' selected from the 'Tools -> Boards' menu."
	#endif

	//the RAMPS board I'm using has a bad Y socket so I use the E2 socket to drive the Y axis

	#define X_STEP_PIN         54
	#define X_DIR_PIN          55
	#define X_ENABLE_PIN       38
	#define X_MIN_PIN           3
	#define X_MAX_PIN           2

	#define Y_STEP_PIN         36
	#define Y_DIR_PIN          34
	#define Y_ENABLE_PIN       30
	#define Y_MIN_PIN          14
	#define Y_MAX_PIN          15   //15

	#define Z_STEP_PIN         46
	#define Z_DIR_PIN          48
	#define Z_ENABLE_PIN       62
	#define Z_MIN_PIN          18
	#define Z_MAX_PIN          19

	#define E_STEP_PIN         26
	#define E_DIR_PIN          28
	#define E_ENABLE_PIN       24

	#define STEPPER_PULSE_WIDTH 2 //Polulu drivers/Allego A4899 chips

	#define E_1_STEP_PIN         60
	#define E_1_DIR_PIN          61
	#define E_1_ENABLE_PIN       56

	#define SDPOWER            -1
	#define SDSS               53
	#define LED_PIN            13
	#define FAN_PIN            9
	#define PS_ON_PIN          12
	#define KILL_PIN           -1
	#define ALARM_PIN          -1

	#define HEATER_0_PIN       10
	#define HEATER_1_PIN        8
	#define HEATER_2_PIN		9
	#define TEMP_0_PIN         13   // ANALOG NUMBERING
	#define TEMP_1_PIN         14   // ANALOG NUMBERING
	#define TEMP_2_PIN         15   // ANALOG NUMBERING
	#define KNOWN_BOARD 1

#endif



/****************************************************************************************
* Sanguino/RepRap Motherboard with direct-drive extruders
*
*                        ATMega644P
*
*                        +---\/---+
*            (D 0) PB0  1|        |40  PA0 (AI 0 / D31)
*            (D 1) PB1  2|        |39  PA1 (AI 1 / D30)
*       INT2 (D 2) PB2  3|        |38  PA2 (AI 2 / D29)
*        PWM (D 3) PB3  4|        |37  PA3 (AI 3 / D28)
*        PWM (D 4) PB4  5|        |36  PA4 (AI 4 / D27)
*       MOSI (D 5) PB5  6|        |35  PA5 (AI 5 / D26)
*       MISO (D 6) PB6  7|        |34  PA6 (AI 6 / D25)
*        SCK (D 7) PB7  8|        |33  PA7 (AI 7 / D24)
*                  RST  9|        |32  AREF
*                  VCC 10|        |31  GND 
*                  GND 11|        |30  AVCC
*                XTAL2 12|        |29  PC7 (D 23)
*                XTAL1 13|        |28  PC6 (D 22)
*       RX0 (D 8)  PD0 14|        |27  PC5 (D 21) TDI
*       TX0 (D 9)  PD1 15|        |26  PC4 (D 20) TDO
*  INT0 RX1 (D 10) PD2 16|        |25  PC3 (D 19) TMS
*  INT1 TX1 (D 11) PD3 17|        |24  PC2 (D 18) TCK
*       PWM (D 12) PD4 18|        |23  PC1 (D 17) SDA
*       PWM (D 13) PD5 19|        |22  PC0 (D 16) SCL
*       PWM (D 14) PD6 20|        |21  PD7 (D 15) PWM
*                        +--------+
*
****************************************************************************************/
#if MOTHERBOARD_MODEL == 1 &&(MOTHERBOARD_VERSION == 1 || MOTHERBOARD_VERSION == 2)
#define KNOWN_BOARD 1

#if !defined(__AVR_ATmega644P__) && !defined(__AVR_ATmega1284P__) 
#error "Oops!  Make sure you have 'Sanguino' selected from the 'Tools -> Boards' menu."
#endif

#define X_STEP_PIN         15
#define X_DIR_PIN          18
#define X_ENABLE_PIN       19
#define X_MIN_PIN          20
#define X_MAX_PIN          21

#define Y_STEP_PIN         23
#define Y_DIR_PIN          22
#define Y_ENABLE_PIN       19
#define Y_MIN_PIN          25
#define Y_MAX_PIN          26

#define Z_STEP_PIN         29
#define Z_DIR_PIN          30
#define Z_ENABLE_PIN       31
#define Z_MIN_PIN           2
#define Z_MAX_PIN           1

#define E_STEP_PIN         12
#define E_DIR_PIN          16
#define E_ENABLE_PIN        3

#define STEPPER_PULSE_WIDTH 2 //???

#define SDPOWER          -1
#define SDSS          -1
#define LED_PIN             0
#define FAN_PIN            -1
#define PS_ON_PIN          -1
#define KILL_PIN           -1
#define ALARM_PIN          -1

#define HEATER_0_PIN       14
#define TEMP_0_PIN          4 //D27   // MUST USE ANALOG INPUT NUMBERING NOT DIGITAL OUTPUT NUMBERING!!!!!!!!!

/*  Unused (1) (2) (3) 4 5 6 7 8 9 10 11 12 13 (14) (15) (16) 17 (18) (19) (20) (21) (22) (23) 24 (25) (26) (27) 28 (29) (30) (31)  */



#endif


/****************************************************************************************
* RepRap Motherboard  ****---NOOOOOO RS485/EXTRUDER CONTROLLER!!!!!!!!!!!!!!!!!---*******
*
****************************************************************************************/
#if MOTHERBOARD_MODEL == 2
#define KNOWN_BOARD 1

#if !defined(__AVR_ATmega644P__) && !defined(__AVR_ATmega1284P__) 
#error Oops!  Make sure you have 'Sanguino' selected from the 'Tools -> Boards' menu.
#endif

#define X_STEP_PIN      15
#define X_DIR_PIN       18
#define X_ENABLE_PIN    19
#define X_MIN_PIN       20
#define X_MAX_PIN       21

#define Y_STEP_PIN      23
#define Y_DIR_PIN       22
#define Y_ENABLE_PIN    24
#define Y_MIN_PIN       25
#define Y_MAX_PIN       26

#define Z_STEP_PINN     27
#define Z_DIR_PINN      28
#define Z_ENABLE_PIN    29
#define Z_MIN_PIN       30
#define Z_MAX_PIN       31

#define E_STEP_PIN      17
#define E_DIR_PIN       16
#define E_ENABLE_PIN    -1

#define STEPPER_PULSE_WIDTH 2 //Polulu drivers/Allego A4899 chips

#define SDPOWER          -1
#define SDSS          4
#define LED_PIN          0

#define SD_CARD_WRITE    2
#define SD_CARD_DETECT   3
#define SD_CARD_SELECT   4

//our RS485 pins
#define TX_ENABLE_PIN	12
#define RX_ENABLE_PIN	13

//pin for controlling the PSU.
#define PS_ON_PIN       14

#define FAN_PIN         -1
#define KILL_PIN        -1
#define ALARM_PIN          -1

#define HEATER_0_PIN    -1
#define TEMP_0_PIN      -1    // MUST USE ANALOG INPUT NUMBERING NOT DIGITAL OUTPUT NUMBERING!!!!!!!!!




#endif

/****************************************************************************************
* Gen3 PLUS for RepRap Motherboard V1.2
*
****************************************************************************************/
#if (MOTHERBOARD_MODEL == 2) && (MOTHERBOARD_VERSION == 1)
#define KNOWN_BOARD 1

#ifndef __AVR_ATmega644P__
    #error Oops!  Make sure you have 'Sanguino' selected from the 'Tools -> Boards' menu.
#endif


//x axis pins
#define X_STEP_PIN      15
#define X_DIR_PIN       18
#define X_ENABLE_PIN    19
#define X_MIN_PIN       20
#define X_MAX_PIN       -1

//y axis pins
#define Y_STEP_PIN      23
#define Y_DIR_PIN       22
#define Y_ENABLE_PIN    24
#define Y_MIN_PIN       25
#define Y_MAX_PIN       -1

//z axis pins
#define Z_STEP_PIN      27
#define Z_DIR_PIN       28
#define Z_ENABLE_PIN    29
#define Z_MIN_PIN       30
#define Z_MAX_PIN       -1

#define E_DIR_PIN       21
#define E_STEP_PIN	17
#define E_ENABLE_PIN	13

#define STEPPER_PULSE_WIDTH 2 //Polulu drivers/Allego A4899 chips

//heaters

//pin for hot end heater
#define HEATER_0_PIN	12

//Pin for heated bed heater
#define HEATER_1_PIN	 16


//pin for debugging.
#define DEBUG_PIN        -1

//SD card pin

#define SDSS		  4

#define SDPOWER          -1
#define FAN_PIN          -1
#define TEMP_0_PIN        0
#define TEMP_1_PIN        5
#define LED_PIN          -1

//pin for controlling the PSU.
#define PS_ON_PIN       14

#endif

/****************************************************************************************
* Gen3  Monolithic Electronics 
*
****************************************************************************************/
#if MOTHERBOARD_MODEL == 2 && MOTHERBOARD_VERSION == 2
#define KNOWN_BOARD 1

#ifndef __AVR_ATmega644P__
    #error Oops!  Make sure you have 'Sanguino' selected from the 'Tools -> Boards' menu.
#endif

#define DEBUG_PIN 0


// x axis
#define X_STEP_PIN 15
#define X_DIR_PIN 18
#define X_MIN_PIN 20
#define X_ENABLE_PIN 24 //actually uses Y_enable_pin
#define X_MAX_PIN -1

// y axes
#define Y_STEP_PIN 23
#define Y_DIR_PIN 22
#define Y_MIN_PIN 25
#define Y_ENABLE_PIN 24 //shared with X_enable_pin 
#define Y_MAX_PIN -1

// z axes
#define Z_STEP_PIN 27
#define Z_DIR_PIN 28
#define Z_MIN_PIN 30
#define Z_ENABLE_PIN 29
#define Z_MAX_PIN -1

//extruder pins
#define E_STEP_PIN 12 
#define E_DIR_PIN 17
#define E_ENABLE_PIN 3

#define STEPPER_PULSE_WIDTH 2 //Polulu drivers/Allego A4899 chips

#define HEATER_0_PIN 16
#define TEMP_0_PIN 0

#define FAN_PIN -1

//bed pins
#define HEATER_1_PIN -1
#define TEMP_1_PIN -1


#define SDSS		 -1
#define SDPOWER          -1
#define LED_PIN          -1

//pin for controlling the PSU.
#define PS_ON_PIN       14

#endif


/****************************************************************************************
* Gen3 PLUS for TechZone Gen3 Remix Motherboard
*
****************************************************************************************/
#if MOTHERBOARD_MODEL == 2 && MOTHERBOARD_VERSION == 3
#define KNOWN_BOARD 1

#ifndef __AVR_ATmega644P__
    #error Oops!  Make sure you have 'Sanguino' selected from the 'Tools -> Boards' menu.
#endif


//x axis pins
#define X_STEP_PIN      15
#define X_DIR_PIN       18
#define X_ENABLE_PIN    24 //same as E/Y_enable_pin
#define X_MIN_PIN       20
#define X_MAX_PIN       -1

//y axis pins
#define Y_STEP_PIN      23
#define Y_DIR_PIN       22
#define Y_ENABLE_PIN    24 //same as E/X_enable_pin
#define Y_MIN_PIN       25
#define Y_MAX_PIN       -1

//z axis pins
#define Z_STEP_PIN      27
#define Z_DIR_PIN       28
#define Z_ENABLE_PIN    29
#define Z_MIN_PIN       30
#define Z_MAX_PIN       -1

#define E_DIR_PIN        21
#define E_STEP_PIN	19
#define E_ENABLE_PIN	24 //same as X/Y_enable_pin

#define STEPPER_PULSE_WIDTH 2 //Polulu drivers/Allego A4899 chips

//heaters

//pin for hot end heater
#define HEATER_0_PIN	16

//Pin for heated bed heater
#define HEATER_1_PIN	17


//pin for debugging.
#define DEBUG_PIN        -1

//SD card pin

#define SDSS		  4

#define SDPOWER          -1
#define FAN_PIN          -1
#define TEMP_0_PIN        0
#define TEMP_1_PIN        5
#define LED_PIN          -1

//pin for controlling the PSU.
#define PS_ON_PIN       14

#endif


/****************************************************************************************
* Arduino Mega/RAMPS pin assignment
*
****************************************************************************************/
#if MOTHERBOARD_MODEL == 3 &&(MOTHERBOARD_VERSION == 1 || MOTHERBOARD_VERSION == 2)
	#define KNOWN_BOARD 1

	#ifndef __AVR_ATmega1280__
	 #ifndef __AVR_ATmega2560__
	 #error Oops!  Make sure you have 'Arduino Mega' selected from the 'Tools -> Boards' menu.
	 #endif
	#endif

	#define X_STEP_PIN		26
	#define X_DIR_PIN 		28
	#define X_ENABLE_PIN 	24
	#define X_MIN_PIN 		 3
	#define X_MAX_PIN 		 2

	#define Y_STEP_PIN 		38
	#define Y_DIR_PIN 		40
	#define Y_MIN_PIN 		16
	#define Y_MAX_PIN 		17
	#define Y_ENABLE_PIN 	36

	#define Z_STEP_PIN 		44
	#define Z_DIR_PIN 		46
	#define Z_MIN_PIN 		18
	#define Z_MAX_PIN 		19
	#define Z_ENABLE_PIN 	42

	#define E_STEP_PIN      32
	#define E_DIR_PIN       34
	#define E_ENABLE_PIN    30

	#define E2_STEP_PIN		23
	#define E2_DIR_PIN 		25
	#define E2_ENABLE_PIN	27

	#define STEPPER_PULSE_WIDTH 2 //Polulu drivers/Allego A4899 chips

	#define LED_PIN            13
	#define PS_ON_PIN          12
	#define KILL_PIN           -1
	#define ALARM_PIN          -1
	#define FAN_PIN            9

	#define HEATER_0_PIN  		10
	#define HEATER_1_PIN  	   	 8
	#define HEATER_2_PIN 	   	 9
	#define TEMP_2_PIN 			 8  //temp_2 = second extruder // -1 in 1.0
	#define TEMP_1_PIN 			 1
	#define TEMP_0_PIN           2   // ANALOG NUMBERING
	#define FAN_PIN   			 9

#endif

//                    RAMPS 1.0
#if MOTHERBOARD_MODEL == 3 && MOTHERBOARD_VERSION == 0
	#define KNOWN_BOARD 1

	#ifndef __AVR_ATmega1280__
	 #ifndef __AVR_ATmega2560__
	 #error Oops!  Make sure you have 'Arduino Mega' selected from the 'Tools -> Boards' menu.
	 #endif
	#endif

	#define X_STEP_PIN		26
	#define X_DIR_PIN 		28
	#define X_ENABLE_PIN 	24
	#define X_MIN_PIN 		 3
	#define X_MAX_PIN 		 2

	#define Y_STEP_PIN 		38
	#define Y_DIR_PIN 		40
	#define Y_MIN_PIN 		16
	#define Y_MAX_PIN 		17
	#define Y_ENABLE_PIN 	36

	#define Z_STEP_PIN 		44
	#define Z_DIR_PIN 		46
	#define Z_MIN_PIN 		18
	#define Z_MAX_PIN 		19
	#define Z_ENABLE_PIN 	42

	#define E_STEP_PIN      32
	#define E_DIR_PIN       34
	#define E_ENABLE_PIN    30

	#define E2_STEP_PIN		-1
	#define E2_DIR_PIN 		-1
	#define E2_ENABLE_PIN	-1

	#define STEPPER_PULSE_WIDTH 2 //Polulu drivers/Allego A4899 chips

	#define LED_PIN            13
	#define PS_ON_PIN          -1
	#define KILL_PIN           -1
	#define ALARM_PIN          -1
	#define FAN_PIN            -1

	#define HEATER_0_PIN  		10
	#define HEATER_1_PIN  	   	-1
	#define HEATER_2_PIN 	   	-1
	#define TEMP_2_PIN 			-1  //temp_2 = second extruder
	#define TEMP_1_PIN 			-1
	#define TEMP_0_PIN          12   // ANALOG NUMBERING
	#define FAN_PIN   			9

#endif

#if MOTHERBOARD_MODEL == 3 && MOTHERBOARD_VERSION == 3
	#if (!defined(__AVR_ATmega1280__)) && (!defined(__AVR_ATmega2560__))
	 	 #error "Oops!  Make sure you have 'Arduino Mega' selected from the 'Tools -> Boards' menu."
	#endif

	#define X_STEP_PIN         54
	#define X_DIR_PIN          55
	#define X_ENABLE_PIN       38
	#define X_MIN_PIN           3
	#define X_MAX_PIN           2

	#define Y_STEP_PIN         60
	#define Y_DIR_PIN          61
	#define Y_ENABLE_PIN       56
	#define Y_MIN_PIN          14
	#define Y_MAX_PIN          15   //15

	#define Z_STEP_PIN         46
	#define Z_DIR_PIN          48
	#define Z_ENABLE_PIN       62
	#define Z_MIN_PIN          18
	#define Z_MAX_PIN          19

	#define E_STEP_PIN         26
	#define E_DIR_PIN          28
	#define E_ENABLE_PIN       24

	#define STEPPER_PULSE_WIDTH 2 //Polulu drivers/Allego A4899 chips

	#define E_1_STEP_PIN         36
	#define E_1_DIR_PIN          34
	#define E_1_ENABLE_PIN       30

	#define SDPOWER            -1
	#define SDSS               53
	#define LED_PIN            13
	#define FAN_PIN            9
	#define PS_ON_PIN          12
	#define KILL_PIN           -1
	#define ALARM_PIN          -1

	#define HEATER_0_PIN       10
	#define HEATER_1_PIN        8
	#define HEATER_2_PIN		9
	#define TEMP_0_PIN         13   // ANALOG NUMBERING
	#define TEMP_1_PIN         14   // ANALOG NUMBERING
	#define TEMP_2_PIN         15   // ANALOG NUMBERING
	#define KNOWN_BOARD 1

#endif
	// SPI for Max6675 Thermocouple 
	  #define MAX6675_SS       49
/****************************************************************************************
* Duemilanove w/ ATMega328P pin assignment
*
****************************************************************************************/
#if MOTHERBOARD_MODEL == 4
#define KNOWN_BOARD 1

#ifndef __AVR_ATmega328P__
#error Oops!  Make sure you have 'Arduino Duemilanove w/ ATMega328' selected from the 'Tools -> Boards' menu.
#endif

#define X_STEP_PIN         19
#define X_DIR_PIN          18
#define X_ENABLE_PIN       -1
#define X_MIN_PIN          17
#define X_MAX_PIN          -1

#define Y_STEP_PIN         10
#define Y_DIR_PIN           7
#define Y_ENABLE_PIN       -1
#define Y_MIN_PIN           8
#define Y_MAX_PIN          -1

#define Z_STEP_PIN         13
#define Z_DIR_PIN           3
#define Z_ENABLE_PIN        2
#define Z_MIN_PIN           4
#define Z_MAX_PIN          -1

#define E_STEP_PIN         11
#define E_DIR_PIN          12
#define E_ENABLE_PIN       -1

#define STEPPER_PULSE_WIDTH 2 //Polulu drivers/Allego A4899 chips

#define LED_PIN            -1
#define FAN_PIN             5
#define PS_ON_PIN          -1
#define KILL_PIN           -1
#define ALARM_PIN          -1

#define HEATER_0_PIN        6
#define TEMP_0_PIN          0    // MUST USE ANALOG INPUT NUMBERING NOT DIGITAL OUTPUT NUMBERING!!!!!!!!!


#endif

/****************************************************************************************
* Gen6 pin assignment
*
****************************************************************************************/
#if MOTHERBOARD == 5
#define KNOWN_BOARD 1

#if !defined(__AVR_ATmega644P__) && !defined(__AVR_ATmega1284P__) 
    #error Oops!  Make sure you have 'Sanguino' selected from the 'Tools -> Boards' menu.
#endif

//x axis pins
    #define X_STEP_PIN      15
    #define X_DIR_PIN       18
    #define X_ENABLE_PIN    19
    #define X_MIN_PIN       20
    #define X_MAX_PIN       -1
    
    //y axis pins
    #define Y_STEP_PIN      23
    #define Y_DIR_PIN       22
    #define Y_ENABLE_PIN    24
    #define Y_MIN_PIN       25
    #define Y_MAX_PIN       -1
    
    //z axis pins
    #define Z_STEP_PIN      27
    #define Z_DIR_PIN       28
    #define Z_ENABLE_PIN    29
    #define Z_MIN_PIN       30
    #define Z_MAX_PIN       -1
    
    //extruder pins
    #define E_STEP_PIN      4     //Edited @ EJE Electronics 20100715
    #define E_DIR_PIN       2     //Edited @ EJE Electronics 20100715
    #define E_ENABLE_PIN    3     //Added @ EJE Electronics 20100715

	#define STEPPER_PULSE_WIDTH 2 //Texas Instruments drivers/DRV8811 chips

    #define TEMP_0_PIN      5     //changed @ rkoeppl 20110410
    #define HEATER_0_PIN    14    //changed @ rkoeppl 20110410
    #define HEATER_1_PIN    -1    //changed @ rkoeppl 20110410
    
    #define LED_PIN         -1    //changed @ rkoeppl 20110410
    #define TEMP_1_PIN      -1    //changed @ rkoeppl 20110410
    #define FAN_PIN         -1    //changed @ rkoeppl 20110410
    #define PS_ON_PIN       -1    //changed @ rkoeppl 20110410
    //our pin for debugging.
    
    #define DEBUG_PIN        0
    
    //our RS485 pins
    #define TX_ENABLE_PIN	12
    #define RX_ENABLE_PIN	13

#endif
/****************************************************************************************
* Sanguinololu pin assignment
*
****************************************************************************************/
#if MOTHERBOARD == 62
#define MOTHERBOARD 6
#define SANGUINOLOLU_V_1_2 
#endif
#if MOTHERBOARD == 6
#define KNOWN_BOARD 1
#if !defined(__AVR_ATmega644P__) && !defined(__AVR_ATmega1284P__) 
#error Oops!  Make sure you have 'Sanguino' selected from the 'Tools -> Boards' menu.
#endif

#define X_STEP_PIN         15
#define X_DIR_PIN          21
#define X_MIN_PIN          18
#define X_MAX_PIN           -2

#define Y_STEP_PIN         22
#define Y_DIR_PIN          23
#define Y_MIN_PIN          19
#define Y_MAX_PIN          -1

#define Z_STEP_PIN         3
#define Z_DIR_PIN          2
#define Z_MIN_PIN          20
#define Z_MAX_PIN          -1

#define E_STEP_PIN         1
#define E_DIR_PIN          0

#define STEPPER_PULSE_WIDTH 2 //Polulu drivers/Allego A4899 chips

#define LED_PIN            -1

#define FAN_PIN            -1 

#define PS_ON_PIN          -1
#define KILL_PIN           -1
#define ALARM_PIN          -1

#define HEATER_0_PIN       13 // (extruder)

#ifdef SANGUINOLOLU_V_1_2

#define HEATER_1_PIN       12 // (bed)
#define X_ENABLE_PIN       14
#define Y_ENABLE_PIN       14
#define Z_ENABLE_PIN       26
#define E_ENABLE_PIN       14

#else

#define HEATER_1_PIN       14  // (bed)
#define X_ENABLE_PIN       -1
#define Y_ENABLE_PIN       -1
#define Z_ENABLE_PIN       -1
#define E_ENABLE_PIN       -1

#endif

#define TEMP_0_PIN          7   // MUST USE ANALOG INPUT NUMBERING NOT DIGITAL OUTPUT NUMBERING!!!!!!!!! (pin 33 extruder)
#define TEMP_1_PIN          6   // MUST USE ANALOG INPUT NUMBERING NOT DIGITAL OUTPUT NUMBERING!!!!!!!!! (pin 34 bed)
#define SDPOWER          -1
#define SDSS          31

#endif

/****************************************************************************************
* Teensylu 0.7 pin assingments (ATMEGA90USB)
* Requires the Teensyduino software with Teensy2.0++ selected in arduino IDE!
****************************************************************************************/
#if MOTHERBOARD == 8
#define MOTHERBOARD 8
#define KNOWN_BOARD 1


#define X_STEP_PIN          0  
#define X_DIR_PIN           1  
#define X_ENABLE_PIN       39 
#define X_MIN_PIN          13 
#define X_MAX_PIN          -1    

#define Y_STEP_PIN          2  
#define Y_DIR_PIN           3 
#define Y_ENABLE_PIN       38 
#define Y_MIN_PIN          14 
#define Y_MAX_PIN          -1    

#define Z_STEP_PIN          4
#define Z_DIR_PIN           5 
#define Z_ENABLE_PIN       23 
#define Z_MIN_PIN          15 
#define Z_MAX_PIN          -1    

#define E_STEP_PIN          6  
#define E_DIR_PIN           7 
#define E_ENABLE_PIN       19 

#define STEPPER_PULSE_WIDTH 2 //Polulu drivers/Allego A4899 chips

#define HEATER_0_PIN       21  // Extruder
#define HEATER_1_PIN       20  // Bed
#define FAN_PIN            22  // Fan   

#define TEMP_0_PIN          7  // Extruder
#define TEMP_1_PIN          6  // Bed

#define SDPOWER            -1
#define SDSS                8
#define LED_PIN            -1
#define PS_ON_PIN          -1
#define KILL_PIN           -1 
#define ALARM_PIN          -1

#ifndef SDSUPPORT
// these pins are defined in the SD library if building with SD support  
  #define SCK_PIN           9 
  #define MISO_PIN         11 
  #define MOSI_PIN         10 
#endif

#endif

/****************************************************************************************
* Gen7 Electronics
* v1.2 7 v1.3 ONLY
****************************************************************************************/
#if MOTHERBOARD == 73
#define KNOWN_BOARD 1

// Gen7 uses a single enable pin for all motors
#define X_ENABLE_PIN       24
#define Y_ENABLE_PIN       24 
#define Z_ENABLE_PIN       24 
#define E_ENABLE_PIN       19 

#define X_STEP_PIN         19 
#define X_DIR_PIN          18 
#define X_MIN_PIN          7 
#define X_MAX_PIN          6    

#define Y_STEP_PIN         23  
#define Y_DIR_PIN          22 
#define Y_MIN_PIN          5 
#define Y_MAX_PIN          2    

#define Z_STEP_PIN         26
#define Z_DIR_PIN          25 
#define Z_MIN_PIN          1 
#define Z_MAX_PIN          0    

#define E_STEP_PIN          6  
#define E_DIR_PIN           7 

#define STEPPER_PULSE_WIDTH 2 //Polulu drivers/Allego A4899 chips

#define HEATER_0_PIN       4  // Extruder
#define HEATER_1_PIN       4  // Bed
#define FAN_PIN            -1  // Fan--only difference from 1.2   

#define TEMP_0_PIN         1  // Extruder
#define TEMP_1_PIN         2  // Bed

#define LED_PIN            -1
#define PS_ON_PIN          15
#define KILL_PIN           -1 
#define ALARM_PIN          -1

#endif

#if MOTHERBOARD == 72
#define KNOWN_BOARD 1

// Gen7 uses a single enable pin for all motors
#define X_ENABLE_PIN       24
#define Y_ENABLE_PIN       24 
#define Z_ENABLE_PIN       24 
#define E_ENABLE_PIN       19 

#define X_STEP_PIN         19 
#define X_DIR_PIN          18 
#define X_MIN_PIN          7 
#define X_MAX_PIN          6    

#define Y_STEP_PIN         23  
#define Y_DIR_PIN          22 
#define Y_MIN_PIN          5 
#define Y_MAX_PIN          2    

#define Z_STEP_PIN         26
#define Z_DIR_PIN          25 
#define Z_MIN_PIN          1 
#define Z_MAX_PIN          0    

#define E_STEP_PIN          6  
#define E_DIR_PIN           7 

#define STEPPER_PULSE_WIDTH 2 //Polulu drivers/Allego A4899 chips


#define HEATER_0_PIN       4  // Extruder
#define HEATER_1_PIN       4  // Bed
#define FAN_PIN            31 // Fan 

#define TEMP_0_PIN         1  // Extruder
#define TEMP_1_PIN         2  // Bed

#define LED_PIN            -1
#define PS_ON_PIN          15
#define KILL_PIN           -1 
#define ALARM_PIN          -1

#endif


#ifndef SDSUPPORT
// these pins are defined in the SD library if building with SD support  
  #define SCK_PIN           9 
  #define MISO_PIN         11 
  #define MOSI_PIN         10 
#endif


#ifndef KNOWN_BOARD
	#error Unknown MOTHERBOARD value in configuration.h
#endif


//List of pins which to ignore when asked to change by gcode, 0 and 1 are RX and TX, do not mess with those!
const int sensitive_pins[] = {X_STEP_PIN, X_DIR_PIN, X_ENABLE_PIN, X_MIN_PIN, X_MAX_PIN, Y_STEP_PIN, Y_DIR_PIN, Y_ENABLE_PIN, Y_MIN_PIN, Y_MAX_PIN, Z_STEP_PIN, Z_DIR_PIN, Z_ENABLE_PIN, Z_MIN_PIN, Z_MAX_PIN, E_STEP_PIN, E_DIR_PIN, E_ENABLE_PIN, LED_PIN, PS_ON_PIN, HEATER_0_PIN, HEATER_1_PIN, FAN_PIN, TEMP_0_PIN, TEMP_1_PIN};

#endif


