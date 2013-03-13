#ifdef AEONS_H

#else
#define AEONS_H

#include "code.h"
#include "AEONS_Config.h"

code * gcode_factory();

void manage_temperatures();

void get_next_command(char * buffer, int buffer_length);

bool verify(int n_value, int checksum_from_command, char* command);

double get_value_from_char_array(char * code, char target);

bool get_value_from_char_array_bool(char * code, char target, float * return_value);

bool test_for_char(char * code, char target);

void fix_comments(char * command);

void clear_command();

void init_pins(); //eventually this will be removed once everything inits its own pins

#endif
