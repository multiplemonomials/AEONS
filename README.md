AEONS Firmware

1. Compilation Instructions
-------------------------------------------------------------------------------------------
1. Download the Arduino IDE v1.0.3.

2. Install Notepad ++.  It isn't required but it is incredibly useful.

3. Extract the contents of the /src folder to Arduino-1.0.3/libraries/AEONS

4. Open the AEONS_Config.h file with Notepad ++.

5. Follow the comments to select your motherboard and thermistor

6. Open the Printer.cpp file.

7. Enter in your AXIS_STEPS_PER_MM

8. Scroll down and set your AXIS_MAX_LENGTHs

9. Launch arduino.exe and select your board from the tools/boards menu

10. Rename "AEONS.cpp" to "AEONS.ino". Open AEONS.ino with Arduino 1.0.3

11. Download the software to the board

11. Troubleshoot.  Depending on your printer, you may need to edit the configuration file and adjust settings.  
	If you get ASSERT errors, check the line number of the firmware included in the error message for a description of the problem.


2. Method/Object Descriptions:
-------------------------------------------------------------------------------------------
Printer AKA Printer::instance()	|| Stores variables and objects about the printer, like heater objects and settings. 

