#ifdef AEONS_H

#else
#define AEONS_H

#include "code.h"
#include "AEONS_Config.h"

code * gcode_factory();

void manage_temperatures();

void get_next_command(char * buffer, int buffer_length);

unsigned int getCurrentExtruderTemperature();

void init_pins(); //eventually this will be removed once everything inits its own pins

#endif
