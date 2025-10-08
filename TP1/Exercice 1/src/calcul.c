#include "../inc/calcul.h"

double eval_polynome(int n, double* tab, double x) {
    /*
    On présuppose que n est positif, et que tab pointe sur une adresse de tableau de n+1 éléments valide.
    */
    double sum = 0;
    double exp = 1;
    for (int i = 0; i < n + 1; i++) {
        sum += exp * tab[i];
        exp *= x;
    }
    return sum;
}