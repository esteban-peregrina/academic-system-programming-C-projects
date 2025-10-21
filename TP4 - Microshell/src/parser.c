#include <stdio.h>

int count_arguments(char buffer[]) {
    int arg_count = 0;
    int i = 0;
    
    while(buffer[i] != '\0') {
        if (buffer[i] != ' ') {
            if(buffer[i+1] == ' ' || buffer[i+1] == '\0') arg_count++;
            i++;
        } else while (buffer[i] == ' ' && buffer[i] != '\0') i++;
    }

    printf("ARGC: %d\n", arg_count);
    
    return arg_count;
}