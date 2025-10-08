#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

#include "image.h"
#include "transf_image.h"

// Gestionnaire de signaux
void gestionnaire(int signum) {
    if (signum == SIGUSR1) {
        printf("Signal SIGUSR1 reçu !\n");
    }
}

void display_usage(){
	printf("\nUsage : ./question4 nom_fichier_origine --bruit pourcentage nom_fichier_destination\n\n");
}

int main(int argc, char **argv){
	struct sigaction my_sigact;<

	image_t *img = NULL;
	unsigned long t0, tf;
	unsigned int pcent;

	/* Test des arguments du main */
	if((argc != 5) || strcmp(argv[2],"--bruit")){
		display_usage();
		return -1;
	}
	
	/* Récupération du pourcentage de bruit */
	pcent = atoi(argv[3]);
	if((pcent > 100) || (pcent < 0)){
		display_usage();
		return -1;
	}
	
	/* Chargement de l'image d'origine */
	img = charger_image_pgm(argv[1]);
	if(! img){
		display_usage();
		return -1;
	}

    image_t* duplicat = dupliquer_image_vers_nmap_anon(img);

	/* Compteur de temps initial */	
	t0 = get_time();

	/* Appel de la fonction de bruitage */
	pid_t child_PID_tab[4];
    int exit_status = EXIT_FAILURE;

    for (int i = 0; i < 4; i++) {
        child_PID_tab[i] = fork();
        if (child_PID_tab[i] == 0) {
            // Code enfant
            bruit_image_zone(duplicat, pcent, i * (duplicat->nx / 4), 0, (i + 1) * (duplicat->nx / 4), duplicat->ny);
            exit(EXIT_SUCCESS);
        } 
        else if (child_PID_tab[i] == -1) exit(EXIT_FAILURE);
    }

    for (int i = 0; i < 4; i++) waitpid(child_PID_tab[i], &exit_status, 0);

	/* Compteur de temps final */
	tf = get_time();
	printf("Operation realisee en %lu microsecondes.\n",(tf-t0));

	/* Sauvegarde de l'image */
	sauver_image_pgm(argv[4], duplicat);

	/* Destruction de l'image en local */
	detruire_image_mmap(duplicat);
	
	return 0;
}

// ---------- QUESTION 6 ---------- //
/* 
On observe que le programme multiprocessus va 4 fois plus vite 
que le programme monoprocessus (57402 ms contre 233114 ms dans mon conteneur sur un Mac 2019).
*/