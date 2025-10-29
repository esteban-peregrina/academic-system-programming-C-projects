#include "../inc/lexer.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define MAX_ARGC 128 // Default 128 TODO

int count_arguments(char buffer[]) {
    int arg_count = 0;
    int i = 0;
    
    while(buffer[i] != '\0') {
        if (buffer[i] != ' ') {
            if(buffer[i+1] == ' ' || buffer[i+1] == '\0') arg_count++;
            i++;
        } else while (buffer[i] != '\0' && buffer[i] == ' ') i++;
    }

    printf("ARGC: %d\n", arg_count);
    
    return arg_count;
}

char** analyze_arg_string(char buffer[], int arg_count) {
    char** arg_values = NULL;
    arg_values = malloc(sizeof(char*) * arg_count);
    if (arg_values == NULL) {
        fprintf(stderr, "Error: malloc failed (%s)\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    const char * delimiter = " ";
    char * strToken = strtok(buffer, delimiter);
    for (int i = 0; i < arg_count; i++) {
        if (strToken == NULL) { break; }
        arg_values[i] = strToken;
        strToken = strtok(NULL, delimiter); // On passe au token suivant
    }

    return arg_values;
}
