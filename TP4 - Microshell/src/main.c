#include "../inc/prompt.h"
#include "../inc/lexer.h"
#include "../inc/parser.h"
#include "../inc/execution.h"

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h> 
#include <fcntl.h>

int main(int argc, char **argv) { // TODO -v verbose et usage
    while(1) {
        // Lecture du prompt
        fprintf(stdout, "€ ");
        char buffer[MAX_LENGTH];
        prompt(buffer);
        //fprintf(stdout, "Prompt is: %s\n", buffer);
        
        // Parsing
        int unit_cmd_count = count_commands(buffer);
        unit_command_t** unit_cmd_array = create_cmd_array(unit_cmd_count); 
        fill_cmd_array(unit_cmd_array, buffer, unit_cmd_count);
        
        // Exécution
        exe_cmd_array(unit_cmd_array, unit_cmd_count);

        // On libère
        for (int i = 0; i < unit_cmd_count; i++) {
            free(unit_cmd_array[i]->token_array); // Even if background ?
            free(unit_cmd_array[i]);
            // Pas besoin de free les raw_command, car ils pointent sur le buffer statique, fractionné (dans le sens d'une string qui finie par '\0') par strtok()
        }
        free(unit_cmd_array);
    }

    return 0;
}
