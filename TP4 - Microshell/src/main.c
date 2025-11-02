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
        if(!unit_command->async) { waitpid(pid, &status, 0); }
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
        int unit_cmd_count = 1; // La dernière après le séparateur
        for (char* c = buffer; *c != '\0'; c++) { if (*c == ';' || *c == '|' || *c == '>' || *c == '&') { unit_cmd_count++; } }
        // On construit le tableau des commandes unitaires avec ce nombre
        unit_command_t** unit_cmd_array = NULL;
        unit_cmd_array = malloc(sizeof(unit_command_t*) * unit_cmd_count);
        if (unit_cmd_array == NULL) {
            fprintf(stderr, "Error: malloc failed (%s)\n", strerror(errno));
            exit(EXIT_FAILURE);
        }

        // On ajoute les commandes unitaires
        for (int i = 0; i < unit_cmd_count; i++) {
            // Allocation
            unit_command_t* unit_command = NULL;
            unit_command = malloc(sizeof(unit_command_t));
            if (unit_command == NULL) {
                fprintf(stderr, "Error: malloc failed (%s)\n", strerror(errno));
                exit(EXIT_FAILURE);
            }

            // Initialisations
            unit_command->async = 0;
            unit_command->separator = SEP_NONE;
            unit_command->raw_command = NULL;
            
            // Ajout dans le tableau de commandes unitaire
            unit_cmd_array[i] = unit_command;
        }

        // On ajoute les séparateurs en reparcourant le buffer
        int separator = 0;
        for (char* c = buffer; *c != '\0'; c++) { 
            if (*c == ';') {unit_cmd_array[separator++]->separator = SEP_SEQUENCE; }
            else if (*c == '|') {unit_cmd_array[separator++]->separator = SEP_PIPE; }
            else if (*c == '&') {unit_cmd_array[separator]->separator = SEP_BACKGROUND; unit_cmd_array[separator++]->async = 1; } // Ligne tricky haha
            else if (*c == '>') {unit_cmd_array[separator++]->separator = SEP_REDIRECT; } // TODO unit_command->outfile
        }
        unit_cmd_array[separator]->separator = SEP_NONE; // La dernière commande

        // On ajoute chaque séquence à sa commande unitaire
        int c = 0;
        const char * delimiter = ";|>&";
        char* command = strtok(buffer, delimiter);
        for (int i = 0; i < unit_cmd_count; i++) {
            if (command == NULL) { break; }
            unit_cmd_array[c++]->raw_command = command;
            command = strtok(NULL, delimiter); // On passe à la commande suivante
        }

        // On exécute tout ce qui est dans le tableau
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
            
            exec_unit_command(unit_cmd_array[i]); //TODO si pipe faut lancer i et i+1 et incrémenter i en plus de la boucle 
        }

        // On libère
        for (int i = 0; i < unit_cmd_count; i++) {
            free(unit_cmd_array[i]->token_array); // Even if background ?
            free(unit_cmd_array[i]);
            // Pas besoin de free les raw_command, car ils pointent sur le buffer, fractionné par strtok()
        }
        free(unit_cmd_array);
    }

    return 0;
}
