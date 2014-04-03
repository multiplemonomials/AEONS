/*
 * Parser.h
 *
 *  Created on: Mar 3, 2014
 *      Author: Jamie
 */

#ifndef PARSER_H_
#define PARSER_H_

#include <stdlib.h>
#include <string.h>
#include <Arduino.h>

#include "AEONS_Config.h"

bool verify(int n_value, int checksum_from_command, char* command);

double find_double_in_command(char * code, char target);

long find_long_in_command(char * code, char target);

bool test_for_char(char * code, char target);

void remove_comments(char * command);

void clear_command(char* command);

#endif /* PARSER_H_ */
