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
#include "Delayer.h"

class Movement
{
public:

#ifdef DEBUG_MOVEMENT
	unsigned long _start_millis;
#endif

	float _x_target;
	float _y_target;
	float _z_target;
	float _e_target;
	float _feedrate;

	StepCount _total_x_steps;
	StepCount _total_y_steps;
	StepCount _total_z_steps;
	StepCount _total_e_steps;

	uint16_t _proposed_x_feedrate;
	uint16_t _proposed_y_feedrate;
	uint16_t _proposed_z_feedrate;
	uint16_t _proposed_e_feedrate;

	StepCount _x_movement_factor;
	StepCount _y_movement_factor;
	StepCount _z_movement_factor;
	StepCount _e_movement_factor;

	uint16_t _loops_to_do;

	int32_t _move_distance_in_mm;
	float _feedrate_mm_per_millisecond;
	float _move_time_in_ms;
	float _time_in_ms_per_loop;
	float _global_steps_per_mm;

	uint32_t _calculation_time_millisconds;

	Delayer* _delayer;

	StepCount _x_steps_counter;
	StepCount _y_steps_counter;
	StepCount _z_steps_counter;
	StepCount _e_steps_counter;

	Movement(float x_target, float y_target, float z_target, float e_target, float feedrate);

	Movement();

	void calculate_values();

	void execute();

	bool is_valid();

	void restrict_to_printable_area();

	void update_current_position();

	void absolute_valueize_direction();

	void mm_to_steps();

	// Enable movement for axes that need to move.
	void enable_axes();

	void limit_feedrates_to_maximums();

	void calculate_movement_ratios();

	void update_endstop_clearances();

	void calculate_delays();

	void generate_accelimap();

	void print_debug_values();

	static uint16_t LCM(unsigned int x_val, unsigned int y_val, unsigned int z_val, unsigned int e_val);

	static uint16_t GCD(unsigned int x_val, unsigned int y_val, unsigned int z_val, unsigned int e_val);

	static uint16_t GCD(unsigned int x_val, unsigned int y_val);


	/*-----------------------------------------------------------------------------
	 	 Multiply operation that always returns a non-zero result, even
	 	 if zero-valued arguments are provided.
	-----------------------------------------------------------------------------*/
	static int multiply_no_0(int value1, int value2);

	void step_loop();

	/*-----------------------------------------------------------------------------
		Modulo operation with special case for zero-valued dividend.
	-----------------------------------------------------------------------------*/
	static int16_t modulo(int dividend, int divisor);
};

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
void move(float x_target, float y_target, float z_target, float e_target, float feedrate);

/*-----------------------------------------------------------------------------

-----------------------------------------------------------------------------*/
void step_loop (Delayer* delayer, unsigned int loop_count, StepCount x_steps_per_tick, StepCount y_steps_per_tick, StepCount z_steps_per_tick, StepCount e_steps_per_tick);

#endif
