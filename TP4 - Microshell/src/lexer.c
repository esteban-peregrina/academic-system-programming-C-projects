#include "../inc/lexer.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define MAX_ARGC 128 // Default 128 TODO

int count_tokens(unit_command_t* unit_command) {
    int i = 0;
    unit_command->token_count = 0;
    while(unit_command->raw_command[i] != '\0') {
        if (unit_command->raw_command[i] != ' ') {
            if(unit_command->raw_command[i+1] == ' ' || unit_command->raw_command[i+1] == '\0') unit_command->token_count++;
            i++;
        } else while (unit_command->raw_command[i] != '\0' && unit_command->raw_command[i] == ' ') i++;
    }

   //fprintf(stdout, "Token count: %d\n", token_count);
    
    return 0;
}

int count_commands(char* prompt) {
    int unit_cmd_count = 1; // On compte la dernière après le dernier séparateur
    for (char* c = prompt; *c != '\0'; c++) { if (*c == ';' || *c == '|' || *c == '>' || *c == '&') { unit_cmd_count++; } }
    // On construit le tableau des commandes unitaires avec ce nombre
    unit_command_t** unit_cmd_array = NULL;
    unit_cmd_array = malloc(sizeof(unit_command_t*) * unit_cmd_count);
    if (unit_cmd_array == NULL) {
        fprintf(stderr, "Error: malloc failed (%s)\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    return unit_cmd_count;
}


