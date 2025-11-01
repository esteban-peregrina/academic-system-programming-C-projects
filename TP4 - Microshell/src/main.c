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
        if(!unit_command->async) { 
            waitpid(pid, &status, 0);
            free(unit_command->token_array); // Seul l'un des deux doit libérer la mémoire //TODO gérer free si background
        }
    }
    return 0;
}

int main(int argc, char **argv) { // TODO -v verbose et usage
    while(1) {
        fprintf(stdout, "€ ");
        char buffer[MAX_LENGTH];
        prompt(buffer);
        //fprintf(stdout, "Prompt is: %s\n", buffer);
        
        // On compte le nombre de commande unitaires composant le prompt
        int unit_cmd_count = 0;
        for (char* c = buffer; *c != '\0'; c++) { if (*c == ';' || *c == '|' || *c == ">" || *c == "&") { unit_cmd_count++; } }
        // On construit le tableau des commandes unitaires avec ce nombre
        unit_command_t** unit_cmd_array = NULL;
        unit_cmd_array = malloc(sizeof(unit_command_t*) * unit_cmd_count);
        if (unit_cmd_array == NULL) {
            fprintf(stderr, "Error: malloc failed (%s)\n", strerror(errno));
            exit(EXIT_FAILURE);
        }

        char* start = buffer;
        for (char* c = buffer; *c != '\0'; c++) {
            unit_command_t* unit_command = NULL;
            unit_command = malloc(sizeof(unit_command_t));
            if (unit_command == NULL) {
                fprintf(stderr, "Error: malloc failed (%s)\n", strerror(errno));
                exit(EXIT_FAILURE);
            }

            if (*c == ';' || *c == '|' || *c == ">" || *c == "&") {
                if (*c == ';') {unit_command->separator = SEP_SEQUENCE; }
                if (*c == '|') {unit_command->separator = SEP_PIPE; }
                if (*c == '&') {unit_command->separator = SEP_BACKGROUND; unit_command->async = 1; }
                if (*c == '>') {unit_command->separator = SEP_REDIRECT; } // TODO unit_command->outfile
                *c = '\0';
                unit_command->raw_command = strdup(start); // ALLOCATION DYNAMIQUE !
                start = *c++;
            }
        }

        // On exécute
        for (int i = 0; i < unit_cmd_count; i++) {
            count_tokens(unit_cmd_array[i]);
            //fprintf(stdout, "Number of tokens: %d\n", unit_cmd_array[i]->token_count);
            analyse_unit_command(unit_cmd_array[i]);
            //for (int i = 0; i < unit_cmd_array->token_count; i++) fprintf(stdout, "%s\n", unit_cmd_array->token_array[i]);
            
            if (strcmp(unit_cmd_array[i]->token_array[0], "exit") == 0) { 
                free(unit_cmd_array[i]->token_array);
                printf("Fermeture du shell...\n");
                exit(EXIT_SUCCESS); 
            }
            
            exec_unit_command(unit_cmd_array[i]);
            exec_unit_command(unit_cmd_array[i]); //TODO si pipe faut lancer i et i+1 et incrémenter i en plus de la boucle 
        }

        // On libère
        for (int i = 0; i < unit_cmd_count; i++) {
            free(unit_cmd_array[i]->raw_command); // TODO C'est tout ?
        }
        free(unit_cmd_array); // TODO free les struct en particulier les strdup()
    }

    return 0;
}
