#include <sys/types.h> 
#include <sys/wait.h> 
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h> // EXIT_FAILURE, EXIT_SUCCESS

int main(int argc, char* argv[]) {
    pid_t parent_PID = getpid();
    printf("Je suis le parent, mon PID est %d.\n\n", parent_PID);

    pid_t child_PID_tab[5];
    int exit_status = EXIT_FAILURE;

    for (int i = 0; i < 5; i++) {
        child_PID_tab[i] = fork();
        
        if (child_PID_tab[i] == -1) exit(EXIT_FAILURE);
        
        else if (child_PID_tab[i] == 0) {
            // Code enfant
            printf("Je suis l'enfant n°%d ! Mon PID est %d et celui de mon parent est %d.\n", i+1, getpid(), getppid());
            exit(EXIT_SUCCESS);
        }
        
        else { 
            // Code parent
            printf("Création de l'enfant n°%d avec PID %d...\n", i+1, child_PID_tab[i]);
            waitpid(child_PID_tab[i], &exit_status, 0);
            if (exit_status == EXIT_FAILURE) printf("L'enfant %d s'est terminé en échec.\n", i);
            else printf("L'enfant %d s'est terminé avec succès.\n\n", i);
        }
    }

    printf("Tous les enfants sont morts. Fin du programme.\n");
    return EXIT_SUCCESS;
}
