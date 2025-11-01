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

int analyse_unit_command(unit_command_t* unit_command) {
    unit_command->token_array = NULL;
    unit_command->token_array = malloc(sizeof(char*) * (unit_command->token_count + 1));
    if (unit_command->token_array == NULL) {
        fprintf(stderr, "Error: malloc failed (%s)\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    const char * delimiter = " ";
    char * strToken = strtok(unit_command->raw_command, delimiter);
    for (int i = 0; i < unit_command->token_count; i++) {
        if (strToken == NULL) { break; }
        unit_command->token_array[i] = strToken;
        strToken = strtok(NULL, delimiter); // On passe au token suivant
    }
    unit_command->token_array[unit_command->token_count] = NULL; // Pour execvp()

    return 0;
}

