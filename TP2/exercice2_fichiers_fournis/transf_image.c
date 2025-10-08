#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include "image.h"
#include "transf_image.h"
#include <unistd.h>

void bruiter_image(image_t *img, unsigned int pourcent){
	srand(time(NULL));
	for (int x = 0; x < img->nx; x++) {
		for (int y = 0; y < img->ny; y++) {
			if (rand()%101 < pourcent) VAL(img, x, y) = rand()%(img->nb_ng + 1);
		}
	}
}

void bruit_image_zone(image_t *img, unsigned int pourcent, unsigned int x_start, unsigned int y_start, unsigned int x_lenght, unsigned int y_lenght) {
	// Utiliser le PID pour avoir une graine différente par processus
	srand(time(NULL) + getpid());
	
	if (x_start > img->nx || x_lenght > img->nx || y_start > img->ny || y_lenght > img->ny) exit(EXIT_FAILURE);
	
	for (int x = x_start; x < x_lenght; x++) {
		for (int y = y_start; y < y_lenght; y++) {
			if (rand() % 101 < pourcent) {
				VAL(img, x, y) = rand() % (img->nb_ng + 1);
			}
		}
	}
}

/* Récupère le temps courant en microsecondes */
unsigned long get_time() {
	struct timeval cTime;
	gettimeofday(&cTime, NULL);
	return ((unsigned long)(1000000*(cTime.tv_sec) + cTime.tv_usec));
}
