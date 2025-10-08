#include <stdio.h> // printf()

#include "../inc/affiche.h"
#include "../inc/calcul.h"

void affiche_polynome(int n, double* tab, double x) {
    double eval = eval_polynome(n, tab, x);
    printf("Résultat de l'evaluation : %lf\n", eval);
}