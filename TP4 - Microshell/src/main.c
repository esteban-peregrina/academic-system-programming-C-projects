#include "../inc/prompt.h"
#include "../inc/parser.h"

#include <stdio.h>

#define MAX_ARGC

int main(int argc, char **argv) { // TODO -v verbose
    char buffer[MAX_LENGTH];
    prompt(buffer);
    fprintf(stdout, "Prompt is: %s\n", buffer);
    int arg_count = count_arguments(buffer);
    printf("ARGC = %d\n", arg_count);
    
    char** arg_values = NULL;

    return 0;
}