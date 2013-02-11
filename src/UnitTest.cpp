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
	ASSERT2(GCD(2, 4) == 2, GCD(2, 4));

	ASSERT(GCD(5, 7) == 1);
	ASSERT(GCD(10, 0) == 10);

	Serial.println("TestGCD completed successfully.");
}


/*-----------------------------------------------------------------------------

-----------------------------------------------------------------------------*/
void TestLCM()
{
	unsigned int value;

	value = LCM(10, 0, 0, 5);
	ASSERT(value == 10);			// typical case
	Serial.println(value);

	value = LCM(5, 10, 20, 100);
	ASSERT(value == 100);
	Serial.println(value);

	value = GCD(264, 1);
	ASSERT(value == 1);
	Serial.println(value);

	value = LCM(0, 265, 0, 0);
	ASSERT(value == 265);
	Serial.println(value);

	////	Serial.print("results of 0000 unit test: ");
////	Serial.println(LCM(0, 0, 0, 0));
////	ASSERT2(LCM(0, 0, 0, 0) 			== 1, LCM(0, 0, 0, 0));			// all zeroes
	ASSERT(LCM(0, 0, 0, 1) 		== 1);			// all zeroes and one 1
//	ASSERT(LCM(5, 7, 0, 0) 			== 35);
//	ASSERT(LCM(3, 5, 7, 11) 		== 3*5*7*11);  	// consecutive primes.
	ASSERT(LCM(1, 1, 1, 1) 			== 1);			// identical arguments
//	ASSERT(LCM(65535 , 3, 0, 0) 	== 65535);     	// largest unsignedint value


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
