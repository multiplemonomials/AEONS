/*----------------------------------__-----------------------------------------
	gcodes.h
	 
	By MultipleMonomials and ChatterComa, thx to Kliment
-----------------------------------------------------------------------------*/

#ifndef GCODES_H
#define GCODES_H

#include "code.h"

/*-----------------------------------------------------------------------------
M80 Turn On Power Supply
-----------------------------------------------------------------------------*/
struct M80 : code
{	
	M80(char * command);
	
	void process();

	virtual ~M80(){}
};

/*-----------------------------------------------------------------------------
M104 (S230) Set extruder temperature to given temp
-----------------------------------------------------------------------------*/
struct M104 : code
{	
	int s_value;
	
	M104(char * command);
	
	void process();

	virtual ~M104(){}
};

/*-----------------------------------------------------------------------------
M105 Turn On Power Supply
-----------------------------------------------------------------------------*/
struct M105 : code
{	
	M105(char * command);
	
	void process();

	virtual ~M105(){}

};

/*------------------------------------------------------------------------------------
M116 [Ssss] [Pppp]-- Wait for extruder and bed to heat up (can specify temps for both)
------------------------------------------------------------------------------------*/

struct M116 : code
{
	int 	s_value;
	int 	p_value;

	M116(char * command);

	void process();

	virtual ~M116(){}

};


#endif

/*-----------------------------------------------------------------------------
M140 <SXXX>--Set bed temperature to given temp
-----------------------------------------------------------------------------*/
struct M140 : code
{
	int s_value;

	M140(char * command);

	void process();

	virtual ~M140(){}

};
