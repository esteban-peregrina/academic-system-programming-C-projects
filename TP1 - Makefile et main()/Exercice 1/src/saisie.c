#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "../inc/saisie.h"

void saisie_valeur_reelle(double* x) {
    float temp;
    char buffer[1024]; // 1 Ko au cas où
    int success;
    do
    {
        printf("Saisissez une valeur réelle pour x : ");
        if (!fgets(buffer, 1024, stdin)) exit(EXIT_FAILURE); 

        char *endptr;
        errno = 0; 
        temp = strtod(buffer, &endptr);
        if (errno == ERANGE) {
            printf("Sorry, this number is too small or too large.\n");
            success = 0;
        }
        else if ((endptr == buffer) || (*endptr && *endptr != '\n')) success = 0;
        else success = 1;
    } while (!success); 
}

void saisie_valeur_entiere(int* n) { 
    long temp;
    char buffer[1024];
    int success;
    do
    {
        printf("Saisissez une valeur entière pour n : ");
        // The following was taken from an article to replace scanf() 
        if (!fgets(buffer, 1024, stdin)) exit(EXIT_FAILURE);

        char *endptr;
        errno = 0;
        temp = strtol(buffer, &endptr, 10);
        if (errno == ERANGE) {
            printf("Sorry, this number is too small or too large.\n");
            success = 0;
        }
        else if ((endptr == buffer) || (*endptr && *endptr != '\n') || (temp < 0)) success = 0;
        else success = 1;
    } while (!success);
}

void saisie_coeff(double** ptr_tab, int* n) {
    printf("Saisie du degré du polynome...\n");
    saisie_valeur_entiere(n);
    printf("Saisie des coefficients du polynome...\n");
    *ptr_tab = malloc(sizeof(double) * (*n + 1));
    double* tab = ptr_tab[0];
    for (int i = 0; i < *n + 1; i++) {
        printf("Coefficient x du degré %d. ", i);
        saisie_valeur_reelle(&tab[i]);
    }
}