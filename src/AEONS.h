#ifdef AEONS_H

#else
#define AEONS_H

#include "code.h"
#include "AEONS_Config.h"

code * gcode_factory();
void manage_temperatures();
void get_next_command(char * buffer, int buffer_length);
void verify(int n_value);
double get_value_from_char_array(char * code, char target);
bool get_value_from_char_array_bool(char * code, char target, float * return_value);
void fix_comments(char * command);
void clear_command();
void init_pins();

#endif
