/*
 * Parser.cpp
 *
 *  Created on: Mar 3, 2014
 *      Author: Jamie
 */

#include "Parser.h"

/*-----------------------------------------------------------------------------
	Returns 0.0 if the specified character is not found in the char[]
	Otherwise returns the number that comes after target in code
-----------------------------------------------------------------------------*/
double find_double_in_command(char * code, char target)
{
	char * pointer_to_target = strchr(code, target);

	if (pointer_to_target == 0)
	{
		return 0;
	}

	char * end_ptr; // strtod returns a double (and a char*) from a char*
	double code_value = strtod((pointer_to_target + 1), &end_ptr);

	return code_value;
}

/*-----------------------------------------------------------------------------
	Returns 0 if the specified character is not found in the char[]
	Otherwise returns the number that comes after target in code
-----------------------------------------------------------------------------*/
long find_long_in_command(char * code, char target)
{
	char * pointer_to_target = strchr(code, target);

	if (pointer_to_target == 0)
	{
		return 0;
	}

	char * end_ptr; // strtod returns a double (and a char*) from a char*
	double code_value = strtoul((pointer_to_target + 1), &end_ptr, 10);

	return code_value;
}

/*-----------------------------------------------------------------------------
	Returns false if the specified character is not found in the char[].
-----------------------------------------------------------------------------*/
bool test_for_char(char * code, char target)
{
	char * pointer_to_target = strchr(code, target);

	return pointer_to_target != 0;
}

/*-----------------------------------------------------------------------------
-----------------------------------------------------------------------------*/
void remove_comments(char * command)
{
	// Find comment delimiter if any.
	char * semicolon_ptr = strchr(command, ';');

	// Truncate command at semicolon.
	if (semicolon_ptr != 0)
	{
		*semicolon_ptr = '\0';
	}
}

/*-----------------------------------------------------------------------------
Null-terminates command at the first character, effectively erasing the\
current text.
-----------------------------------------------------------------------------*/
void clear_command(char* command)
{
		command[0] = '\0';
}



