#ifndef LEXER_H
#define LEXER_H

typedef enum {
    SEP_NONE,
    SEP_SEQUENCE,   // ';'
    SEP_PIPE,   // '|'
    SEP_BACKGROUND, // '&'
    SEP_REDIRECT,   // '>'
} separator_t;

typedef struct unit_command_s {
    char* raw_command;
    int token_count;
    char** token_array;
    separator_t separator;
    int async;
    char* outfile;
} unit_command_t;

int count_tokens(unit_command_t* unit_command);
int count_commands(char* prompt);

#endif // LEXER_H
