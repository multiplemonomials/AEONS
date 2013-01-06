#ifdef AEONS_H

#else
#define AEONS_H

#include "code.h"

code * gcode_factory();
void manage_temperatures();
void get_next_command(char * buffer, int buffer_length);
void validate(int n_value);
double get_value_from_char_array(char * code, char target);
void fix_comments(char * command);
void init_pins();

#endif