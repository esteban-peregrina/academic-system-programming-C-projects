#include "../inc/prompt.h"
#include "../inc/lexer.h"

#include <stdio.h>

#include <errno.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ARGC 128 // Default 128

int main(int argc, char **argv) { // TODO -v verbose et usage
    char buffer[MAX_LENGTH];
    prompt(buffer);
    fprintf(stdout, "Prompt is: %s\n", buffer);
    int arg_count = count_arguments(buffer);
    printf("ARGC = %d\n", arg_count);
    
    char** arg_values = analyze_arg_string(buffer, arg_count);
    for (int i = 0; i < arg_count; i++) printf("%s\n", arg_values[i]);
    free(arg_values);

    return 0;
}