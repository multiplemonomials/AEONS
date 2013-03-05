/*-----------------------------------------------------------------------------
	gcodes.h
	 
	By MultipleMonomials and ChatterComa, thx to Kliment
-----------------------------------------------------------------------------*/

#ifndef GCODES_H
#define GCODES_H

#include "code.h"
#include "AEONS_Config.h"
#include "Arduino.h"
#include "Printer.h"
#include "AEONS.h"
#include "Movement.h"

/*-----------------------------------------------------------------------------
G1--Linear Move
-----------------------------------------------------------------------------*/

struct G1 : code
{
//	bool has_x_value;
	float x_value;

//	bool has_y_value;
	float y_value;

//	bool has_z_value;
	float z_value;

//	bool has_e_value;
	float e_value;

//	bool has_f_value;
	float f_value;

	G1(char * command);

	void process();

	virtual ~G1(){}

};

/*-----------------------------------------------------------------------------
G28 Home All
-----------------------------------------------------------------------------*/
struct G28 : code
{

	G28(char * command);

	void process();
};

/*-----------------------------------------------------------------------------
G90 Set absolute positioning
-----------------------------------------------------------------------------*/
struct G90 : code
{

	G90(char * command);

	void process();

	virtual ~G90(){}

};

/*-----------------------------------------------------------------------------
G91 Set relative positioning
-----------------------------------------------------------------------------*/
struct G91 : code
{

	G91(char * command);

	void process();

	virtual ~G91(){}

};

/*-----------------------------------------------------------------------------
G92 Set Position
-----------------------------------------------------------------------------*/
struct G92 : code
{
	float x_value;
	float y_value;
	float z_value;
	float e_value;

	bool has_x_value;
	bool has_y_value;
	bool has_z_value;
	bool has_e_value;

	G92(char * command);

	void process();

};


/*-----------------------------------------------------------------------------
M40--Eject printed objects by running user-specified commands
-----------------------------------------------------------------------------*/
struct M40 : code
{

	M40(char * command);

	void process();

	virtual ~M40(){}

};

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
M81 Turn Off Power Supply
-----------------------------------------------------------------------------*/
struct M81 : code
{
	M81(char * command);

	void process();

	virtual ~M81(){}
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
M105 Print current temperature
-----------------------------------------------------------------------------*/
struct M105 : code
{	
	M105(char * command);
	
	void process();

	virtual ~M105(){}

};

/*-----------------------------------------------------------------------------
M80 Turn On Power Supply
-----------------------------------------------------------------------------*/
struct M106 : code
{
	M106(char * command);

	void process();

	virtual ~M106(){}
};

/*-----------------------------------------------------------------------------
M107 Turn Off Fan
-----------------------------------------------------------------------------*/
struct M107 : code
{
	M107(char * command);

	void process();

	virtual ~M107(){}
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

