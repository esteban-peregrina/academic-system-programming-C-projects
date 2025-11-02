#include "../inc/execution.h"
#include "../inc/lexer.h"
#include "../inc/parser.h"

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h> 
#include <fcntl.h>

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

int exec_redirect(unit_command_t* unit_command, unit_command_t* next_command) {
    pid_t pid = fork(); // Split process in two, returning 0 to child and parent PID to parent
    if (pid == -1) {
        fprintf(stderr, "Error: fork failed (%s)\n", strerror(errno));
        free(unit_command->token_array);
        free(next_command->token_array);
        exit(EXIT_FAILURE);
    }
    else if (pid == 0) {
        //fprintf(stdout, "I'm the child, my PID is %d !\n", getpid());
        if (next_command && next_command->token_array && next_command->token_array[0]) {
            FILE* fd = fopen(next_command->token_array[0], "w");
            if (fd == NULL) {
                fprintf(stderr, "Error: fopen failed (%s)\n", strerror(errno));
                exit(EXIT_FAILURE);
            }
            if (dup2(fileno(fd), STDOUT_FILENO) == -1) {
                fprintf(stderr, "Error: dup2 failed (%s)\n", strerror(errno));
                fclose(fd);
                exit(EXIT_FAILURE);
            }
            fclose(fd);
        }

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

int exec_pipe(unit_command_t* cmd1, unit_command_t* cmd2) {
    int pipefd[2];
    if (pipe(pipefd) == -1) { // Pipe fournit deux descripteur, une extremite de tuyau reservee pour l'ecriture, l'autre pour la lecture
        fprintf(stderr, "Error: pipe failed (%s)\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    
    pid_t pid1 = fork(); // Split process in two, returning 0 to child and parent PID to parent
    if (pid1 == -1) {
        fprintf(stderr, "Error: fork failed (%s)\n", strerror(errno));
        close(pipefd[0]);
        close(pipefd[1]);
        exit(EXIT_FAILURE);
    }
    
    else if (pid1 == 0) {
        // Premier processus : écrire dans le tuyau
        close(pipefd[0]); // On ferme le descripteur de lecture
        if (dup2(pipefd[1], STDOUT_FILENO) == -1) { // On remplace STDOUT pour rediriger vers pipefd[1] (donc le tuyau)
            fprintf(stderr, "Error: dup2 failed (%s)\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
        close(pipefd[1]); // On ferme le descripteur d'écriture (on a fini de l'exploiter)
        
        // On exécute
        if (execvp(cmd1->token_array[0], cmd1->token_array) == -1) {
            fprintf(stderr, "Error: execvp failed (%s)\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
    }
    
    pid_t pid2 = fork(); // Split process in two, returning 0 to child and parent PID to parent
    if (pid2 == -1) {
        fprintf(stderr, "Error: fork failed (%s)\n", strerror(errno));
        close(pipefd[0]);
        close(pipefd[1]);
        exit(EXIT_FAILURE);
    }
    
    else if (pid2 == 0) {
        // Deuxième processus : lire depuis le tuyau
        close(pipefd[1]); // On ferme le descripteur d'écriture
        if (dup2(pipefd[0], STDIN_FILENO) == -1) {
            fprintf(stderr, "Error: dup2 failed (%s)\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
        close(pipefd[0]); // On ferme le descripteur de lecture (on a fini de l'exploiter)
        
        // On exécute
        if (execvp(cmd2->token_array[0], cmd2->token_array) == -1) {
            fprintf(stderr, "Error: execvp failed (%s)\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
    }
    
    // Processus parent : on ferme les deux extrémités
    close(pipefd[0]);
    close(pipefd[1]);
    
    // On attend les deux processus enfants
    int status;
    waitpid(pid1, &status, 0);
    waitpid(pid2, &status, 0);
    
    return 0;
}

int exe_cmd_array (unit_command_t** unit_cmd_array, int unit_cmd_count) {
    // On exécute tout ce qui est dans le tableau
    for (int i = 0; i < unit_cmd_count; i++) {
        // Built-in "exit"
        if (strcmp(unit_cmd_array[i]->token_array[0], "exit") == 0) { 
            free(unit_cmd_array[i]->token_array);
            printf("Fermeture du shell...\n");
            exit(EXIT_SUCCESS); 
        }

        // Built-in "cd"
        if (strcmp(unit_cmd_array[i]->token_array[0], "cd") == 0) {
            if (unit_cmd_array[i]->token_count < 2) {
                fprintf(stderr, "Error: cd: missing argument\n");
            } else {
                if (chdir(unit_cmd_array[i]->token_array[1]) != 0) {
                    fprintf(stderr, "Error: cd %s: %s\n", unit_cmd_array[i]->token_array[1], strerror(errno));
                }
            }
            continue; // Passe à la commande suivante
        }

        // Built-in "pwd"
        if (strcmp(unit_cmd_array[i]->token_array[0], "pwd") == 0) {
            char cwd[1024];
            if (getcwd(cwd, sizeof(cwd)) == NULL) {
                fprintf(stderr, "Error: getcwd failed %s\n", strerror(errno));
            } else {
                printf("%s\n", cwd);
            }
            continue; // Passe à la commande suivante
        }
        
        // Gestion des pipes
        if (unit_cmd_array[i]->separator == SEP_PIPE && i + 1 < unit_cmd_count) {
            exec_pipe(unit_cmd_array[i], unit_cmd_array[i + 1]);
            i++; // On saute la commande suivante car elle est déjà traitée
        }

        // Gestion des redirections
        else if (unit_cmd_array[i]->separator == SEP_REDIRECT && i + 1 < unit_cmd_count) {
            exec_redirect(unit_cmd_array[i], unit_cmd_array[i + 1]);
            i++; // On saute la commande suivante car elle est déjà traitée
        }

        // Exécution normale
        else {
            if (unit_cmd_array[i]->separator == SEP_PIPE) { fprintf(stdout, "Tuyau négligé car absence de second processus.\n"); }
            if (unit_cmd_array[i]->separator == SEP_REDIRECT) { fprintf(stdout, "Redirection négligée car absence de second processus.\n"); }
            exec_unit_command(unit_cmd_array[i]);
        }
    }

    return 0;
}
