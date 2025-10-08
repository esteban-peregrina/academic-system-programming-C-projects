#include "../inc/prompt.h"

#include <stdlib.h> // NULL, exit(), EXIT_FAILURE, EXIT_SUCESS
#include <stdio.h> // stdin,
#include <errno.h> // errno,
#include <string.h> // strerror(),

#define MAX_LENGTH 1024 // Default : 1024

int prompt() {
    char buffer[MAX_LENGTH];
    if (fgets(buffer, MAX_LENGTH, stdin) == NULL) { // fgets() will bufferize MAX_LENGTH - 1 characters THEN adds '\0' at the end
        if (feof(stdin)) fprintf(stderr, "Error: unexpected end of input\n");
        else fprintf(stderr, "Error: fgets failed (%s)\n", strerror(errno));
        exit(EXIT_FAILURE);
    } 
    
    size_t len = strlen(buffer); // Equals MAX_LENGTH - 1 as strlen() doesn't count the '\0' that might have been added
    if (buffer[len - 1] != '\n') { // Thus, does buffer[MAX_LENGTH - 2]
        char c;
        while ((c = getchar()) != '\n' && c != EOF); // Vide la ligne courante du flux stdin
        fprintf(stderr, 
            "Error: prompt is too long and won't be interpreted.\n"
            "Please provide a prompt that doesn't exceed %d characters including newline.\n" 
            "(string %s was bufferized, but it expects to end with a newline)\n", 
            MAX_LENGTH - 1, buffer);
        exit(EXIT_SUCCESS);
    } else { // buffer[MAX_LENGTH - 2] == '\n' (buffer[MAX_LENGTH - 1] == '\0')
        buffer[len - 1] = '\0';
    }

    fprintf(stdout, "Prompt is: %s\n", buffer);
    
    return 0;
}