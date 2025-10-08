#ifndef TRANSF_IMAGE
#define TRANSF_IMAGE

#include "image.h"

void bruiter_image(image_t *img, unsigned int pourcent);

void bruit_image_zone(image_t *img, unsigned int pourcent, unsigned int x_start, unsigned int y_start, unsigned int x_lenght, unsigned int y_lenght); 

unsigned long get_time();
#endif
