#ifndef EXECUTION_H
#define EXECUTION_H

#include "lexer.h"

int exec_unit_command(unit_command_t* unit_command);
int exec_redirect(unit_command_t* unit_command, unit_command_t* next_command);
int exec_pipe(unit_command_t* cmd1, unit_command_t* cmd2);
int exe_cmd_array (unit_command_t** unit_cmd_array, int unit_cmd_count);

#endif // EXECUTION_H
