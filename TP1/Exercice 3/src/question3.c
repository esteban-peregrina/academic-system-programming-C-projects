#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define MIN_ARG 4 // Sans les coeff, ex : ./question3 --val valeur_evaluation --coef 

// J'ai préfére les copier ppur éviter de complexifier le makfeil, 
// et d'avoir les .c dans toutes les questions en les ajoutant au src 
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

void affiche_polynome(int n, double* tab, double x) {
    /*
    On présuppose que n est positif, et que tab pointe sur une adresse de tableau de n+1 éléments valide.
    */
    double eval = eval_polynome(n, tab, x);
    printf("Résultat de l'evaluation : %lf\n", eval);
}

void display_usage() {
    printf("\nUsage : ./question3 --val valeur_evaluation --coef coef1 coef2 coef3\n\n");
}

// Bon alors celle-là elle est pas mal bullet-proof je vous laisse tester
int main(int argc, char **argv) {
    if (argc < 2) {
        display_usage();
        fprintf(stderr, "Pas assez d'arguments.\n");
        exit(EXIT_FAILURE);
    }

    if ((strcmp(argv[1], "--h") == 0) || (strcmp(argv[1], "--help") == 0)) {
        display_usage();
        exit(EXIT_SUCCESS);
    }

    double val = 0.0;
    double *tab_coef = NULL;
    int nb_coef = 0;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--val") == 0 && i + 1 < argc) { // La seconde condition permet d'éviter de sortir du tableau
            // On réutilise la logique de l'exercice 1 (celle pour palier aux scanf()) pour vérifier la validité des arguments.
            char *endptr;
            errno = 0;
            val = strtod(argv[i + 1], &endptr); // On récupère la valeur (contenue en i + 1)
            if ((errno == ERANGE || *endptr != '\0') || (endptr == argv[i + 1]) || (*endptr && *endptr != '\n')) {
                fprintf(stderr, "Erreur: '%s' n'est pas un réel valide.\n", argv[i + 1]);
                exit(EXIT_FAILURE);
            }
        } else if (strcmp(argv[i], "--coef") == 0 && i + 1 < argc) {
            // On commence par compter le nombre de coefficients
            int start = i + 1;
            nb_coef = 0;
            while (start + nb_coef < argc && strncmp(argv[start + nb_coef], "--", 2) != 0) nb_coef++;

            if (nb_coef == 0) {
                display_usage();
                fprintf(stderr, "Pas assez de coefficients.\n");
                exit(EXIT_FAILURE);
            }

            tab_coef = malloc(sizeof(double) * nb_coef);
            if (!tab_coef) {
                fprintf(stderr, "Impossible d'allouer la mémoire.\n");
                exit(EXIT_FAILURE);
            }

            for (int j = 0; j < nb_coef; j++) {
                char *endptr;
                errno = 0;
                tab_coef[j] = strtod(argv[i + 1 + j], &endptr);
                if ((errno == ERANGE || *endptr != '\0') || (endptr == argv[i + 1 + j]) || (*endptr && *endptr != '\n')) {
                    fprintf(stderr, "Erreur: '%s' n'est pas un réel valide.\n", argv[i + 1 + j]);
                    free(tab_coef); // On lièbre la mémoire avant de quitter
                    exit(EXIT_FAILURE);
                }
            }
            i += nb_coef; // avancer l’index après les coefficients
        }
    }

    affiche_polynome(argc - MIN_ARG + 1, tab_coef, val);

    free(tab_coef);
    return 0;
}
