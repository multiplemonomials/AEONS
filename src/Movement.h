/*-----------------------------------------------------------------------------
	Device.h

	By MultipleMonomials and ChatterComa, thx to Kliment
-----------------------------------------------------------------------------*/

#ifndef MOVEMENT_H
#define MOVEMENT_H

#include "Printer.h"
#include "AEONS.h"
#include "Arduino.h"
#include "AEONS_Typedefs.h"
#include "delay.h"

/*-----------------------------------------------------------------------------

-----------------------------------------------------------------------------*/
unsigned int LCM(unsigned int x_val, unsigned int y_val, unsigned int z_val, unsigned int e_val);

/*-----------------------------------------------------------------------------

-----------------------------------------------------------------------------*/
unsigned int GCD(unsigned int x_val, unsigned int y_val, unsigned int z_val, unsigned int e_val);

/*-----------------------------------------------------------------------------

-----------------------------------------------------------------------------*/
unsigned int GCD(unsigned int x_val, unsigned int y_val);

/*-----------------------------------------------------------------------------

-----------------------------------------------------------------------------*/
void get_delay_from_feedrate(int x_val, int y_val, int z_val, int e_val, int feedrate);

/*-----------------------------------------------------------------------------

-----------------------------------------------------------------------------*/
void move(float x_target, float y_target, float z_target, float e_target, float feedrate);

/*-----------------------------------------------------------------------------

-----------------------------------------------------------------------------*/
void step_loop
(
	delay_base* delayer,
	unsigned int loop_count,
	StepCount x_steps_per_tick,
	StepCount y_steps_per_tick,
	StepCount z_steps_per_tick,
	StepCount e_steps_per_tick
);

#endif
