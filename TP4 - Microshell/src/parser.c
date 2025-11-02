#include "../inc/parser.h"
#include "../inc/lexer.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

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

unit_command_t** create_cmd_array(int unit_cmd_count) {
    unit_command_t** unit_cmd_array = NULL;
    unit_cmd_array = malloc(sizeof(unit_command_t*) * unit_cmd_count);
    if (unit_cmd_array == NULL) {
        fprintf(stderr, "Error: malloc failed (%s)\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    return unit_cmd_array;
}

int fill_cmd_array(unit_command_t** unit_cmd_array, char* prompt, int unit_cmd_count) {
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
    for (char* c = prompt; *c != '\0'; c++) { 
        if (*c == ';') {unit_cmd_array[separator++]->separator = SEP_SEQUENCE; }
        else if (*c == '|') {unit_cmd_array[separator++]->separator = SEP_PIPE; }
        else if (*c == '&') {unit_cmd_array[separator]->separator = SEP_BACKGROUND; unit_cmd_array[separator++]->async = 1; } // Ligne tricky haha
        else if (*c == '>') {unit_cmd_array[separator++]->separator = SEP_REDIRECT; } // TODO unit_command->outfile
    }
    unit_cmd_array[separator]->separator = SEP_NONE; // La dernière commande

    // On ajoute chaque séquence à sa commande unitaire
    int c = 0;
    const char * delimiter = ";|>&";
    char* command = strtok(prompt, delimiter);
    for (int i = 0; i < unit_cmd_count; i++) {
        if (command == NULL) { break; }
        unit_cmd_array[c++]->raw_command = command;
        command = strtok(NULL, delimiter); // On passe à la commande suivante
    }

    return 0;
}
