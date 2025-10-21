#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) { 
    char* all_args[3];
    all_args[0] = "who";
    all_args[1] = "ps";
    all_args[2] = "ls -l";

    pid_t child_PID_tab[3];
    int exit_status = EXIT_FAILURE;

    for (int i = 0; i < 3; i++) {
        child_PID_tab[i] = fork();
        if (child_PID_tab[i] == 0) {
            // Code enfant
            char* my_args[] = {"bash", "-c", all_args[i], NULL};
            execv("/bin/bash", my_args);
            exit(EXIT_FAILURE); // Ne devrait pas arriver là si execv fonctionne correctement
        } 
        else if (child_PID_tab[i] == -1) exit(EXIT_FAILURE);

        else {
            // Code parent
            waitpid(child_PID_tab[i], &exit_status, 0);
        }
    }
    return EXIT_SUCCESS;
}