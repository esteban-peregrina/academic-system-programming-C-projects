#ifndef PARSER_H
#define PARSER_H

int count_arguments(char buffer[]);
char** parse_arg_string(char buffer[], int arg_count);

#endif // PARSER_H