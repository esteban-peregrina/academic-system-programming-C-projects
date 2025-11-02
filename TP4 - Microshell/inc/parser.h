#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"

int analyse_unit_command(unit_command_t* unit_command);
unit_command_t** create_cmd_array(int unit_cmd_count);
int fill_cmd_array(unit_command_t** unit_cmd_array, char* prompt, int unit_cmd_count);

#endif // PARSER_H
