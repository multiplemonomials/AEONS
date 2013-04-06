/*-----------------------------------------------------------------------------
	UnitTest.cpp

	By MultipleMonomials and ChatterComa, thx to Kliment
-----------------------------------------------------------------------------*/

#include "UnitTest.h"
#include "Arduino.h"
#include "Movement.h"
#include "assert.h"


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

	Serial.println("TestALL completed successfully.");
}
