#include "../inc/prompt.h"
#include "../inc/lexer.h"

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h> 

int exec_unit_command(unit_command_t* unit_command) {
    pid_t pid = fork(); // Split process in two, returning 0 to child and parent PID to parent
    if (pid == -1) {
        fprintf(stderr, "Error: fork failed (%s)\n", strerror(errno));
        free(unit_command->token_array);
        exit(EXIT_FAILURE);
    }
    else if (pid == 0) {
        //fprintf(stdout, "I'm the child, my PID is %d !\n", getpid());
        if (execvp(unit_command->token_array[0], unit_command->token_array) == -1) {
            fprintf(stderr, "Error: execvp failed (%s)\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
    } else {
        //fprintf(stdout, "I'm the parent, my PID is %d and my child's PID is %d !\n", getpid(), pid);
        int status;
        waitpid(pid, &status, 0);
        free(unit_command->token_array); // Seul l'un des deux doit libérer la mémoire
    }
    return 0;
}

int main(int argc, char **argv) { // TODO -v verbose et usage
    while(1) {
        fprintf(stdout, "€ ");
        char buffer[MAX_LENGTH];
        prompt(buffer);
        //fprintf(stdout, "Prompt is: %s\n", buffer);
        
        unit_command_t my_command;
        my_command.raw_command = buffer;
        count_tokens(&my_command);
        //fprintf(stdout, "Number of tokens: %d\n", my_command.token_count);
        analyse_unit_command(&my_command);
        //for (int i = 0; i < my_command.token_count; i++) fprintf(stdout, "%s\n", my_command.token_array[i]);
        
        if (strcmp(my_command.token_array[0], "exit") == 0) { 
            free(my_command.token_array);
            printf("Fermeture du shell...\n");
            exit(EXIT_SUCCESS); 
        }
        
        exec_unit_command(&my_command);
    }

    return 0;
}
