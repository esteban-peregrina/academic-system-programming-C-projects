#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "image.h"
#include "transf_image.h"

void display_usage(){
	printf("\nUsage : ./question3 nom_fichier_origine --bruit pourcentage nom_fichier_destination\n\n");
}

int main(int argc, char **argv){

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

	/* Compteur de temps initial */	
	t0 = get_time();

	/* Appel de la fonction de bruitage */
	bruit_image_zone(img, pcent, 50, 80, 100, 100);

	/* Compteur de temps final */
	tf = get_time();
	printf("Operation realisee en %lu microsecondes.\n",(tf-t0));

	/* Sauvegarde de l'image */
	sauver_image_pgm(argv[4],img);

	/* Destruction de l'image en local */
	detruire_image(img);
	
	return 0;
}
