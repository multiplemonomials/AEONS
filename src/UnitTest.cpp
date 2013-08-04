/*-----------------------------------------------------------------------------
	UnitTest.cpp

	By MultipleMonomials and ChatterComa, thx to Kliment
-----------------------------------------------------------------------------*/

#include "UnitTest.h"
#include "Arduino.h"
#include "Movement.h"
#include "assert.h"
#include "AEONS_Config.h"
#include "Printer.h"


/*-----------------------------------------------------------------------------

-----------------------------------------------------------------------------*/
void TestGCD()
{
	ASSERT2(Movement::GCD(2, 4) == 2, Movement::GCD(2, 4));

	ASSERT(Movement::GCD(5, 7) == 1);
	ASSERT(Movement::GCD(10, 0) == 10);

	Serial.println("TestGCD completed successfully.");
}


/*-----------------------------------------------------------------------------

-----------------------------------------------------------------------------*/
void TestLCM()
{
	unsigned int value;

	value = Movement::LCM(10, 0, 0, 5);
	ASSERT(value == 10);			// typical case

	value = Movement::LCM(5, 10, 20, 100);
	ASSERT(value == 100);

	value = Movement::LCM(0, 265, 0, 0);
	ASSERT(value == 265);

	ASSERT(Movement::LCM(0, 0, 0, 1) 		== 1);			// all zeroes and one 1

	ASSERT(Movement::LCM(1, 1, 1, 1) 			== 1);			// identical arguments


	Serial.println("TestLCM completed successfully.");
}


/*-----------------------------------------------------------------------------
	Runs all the unit tests.
-----------------------------------------------------------------------------*/
void TestAll()
{
	TestGCD();
	TestLCM();

	TestMovement();

	Serial.println("TestALL completed successfully.");
}

void TestMovement()
{
	//testPrintableArea();

	//Serial.println("TestMovement completed successfully.");
}

//void testPrintableArea()
//{
//	Movement * movement = new Movement();
//	movement->_x_target = X_MAX_LENGTH + 1;
//	movement->restrict_to_printable_area();
//	Serial.println(movement->_x_target);
//	ASSERT((int)movement->_x_target + (int)Printer::instance().x_axis._current_position <= X_MAX_LENGTH + 1);
//	delete movement;
//}
