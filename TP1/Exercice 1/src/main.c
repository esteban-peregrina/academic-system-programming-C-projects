#include "../inc/affiche.h"
#include "../inc/saisie.h"

#include <stdlib.h>

int main() {
    double* tab;
    int n;
    saisie_coeff(&tab, &n);
    affiche_polynome(n, tab, 10.0);
    free(tab);
    return 0;
}

/*
.
|-- bin
|   `-- program
|-- build
|   |-- affiche.o
|   |-- calcul.o
|   |-- main.o
|   `-- saisie.o
|-- inc
|   |-- affiche.h
|   |-- calcul.h
|   `-- saisie.h
|-- makefile
`-- src
    |-- affiche.c
    |-- calcul.c
    |-- main.c
    `-- saisie.c

5 directories, 13 files

Les fichiers .o du dossier build dépendent des fichiers sources pour être créés, 
et l'executable program dépend de ces fichiers .o pour être créé : 
on va baser notre Makefile sur ces dépendances.
*/