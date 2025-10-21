#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[]) { 
    if (argc != 2) exit(EXIT_FAILURE);
    char my_string[50]; // 50 caractères pour être "large"
    snprintf(my_string, 50, "emacs %s", argv[1]);
    printf("Lancement de %s avec emacs...\n", my_string);
    if (system(my_string) == -1) exit(EXIT_FAILURE);
    printf("Programme terminé.\n");
    return 0;
}