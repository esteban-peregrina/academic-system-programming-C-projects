#include "../inc/prompt.h"
#include "../inc/lexer.h"

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h> 

int main(int argc, char **argv) { // TODO -v verbose et usage
    while(1) {
        char buffer[MAX_LENGTH];
        prompt(buffer);
        fprintf(stdout, "Prompt is: %s\n", buffer);
        int arg_count = count_arguments(buffer);
        printf("ARGC = %d\n", arg_count);
        
        char** arg_values = analyze_arg_string(buffer, arg_count);
        for (int i = 0; i < arg_count; i++) printf("%s\n", arg_values[i]);
        if (strcmp(arg_values[0], "exit") == 0) { 
            free(arg_values);
            printf("Bye 👋\n");
            exit(EXIT_SUCCESS); 
        }

        pid_t pid = fork(); // Split process in two, returning 0 to child and parent PID to parent
        if (pid == -1) {
            fprintf(stderr, "Error: fork failed (%s)\n", strerror(errno));
            free(arg_values);
            exit(EXIT_FAILURE);
        }
        else if (pid == 0) {
            fprintf(stdout, "I'm the child, my PID is %d !\n", getpid());
            if (execvp(arg_values[0], arg_values) == -1) {
                fprintf(stderr, "Error: execvp failed (%s)\n", strerror(errno));
                exit(EXIT_FAILURE);
            }

            // On ne doit jamais arriver ici si execvp() à correctement ecrasé le processus enfant
            fprintf(stderr, "Error: execvp failed (%s)\n", strerror(errno));
            exit(EXIT_FAILURE);
        } else {
            int status;
            waitpid(pid, &status, 0);
            free(arg_values);
            fprintf(stdout, "I'm the parent, my PID is %d and my child's PID is %d !\n", getpid(), pid);
        }
    }

    return 0;
}
