#ifndef IMAGE
#define IMAGE

#define VAL(img,i,j) (img)->buff[(j)*(img)->nx+(i)]

typedef struct{
  unsigned int nx;
  unsigned int ny;
  unsigned int nb_ng;
  unsigned char *buff;
} image_t;

image_t *creer_image();

image_t *allouer_image(image_t *p_im,
		       unsigned int nx,
		       unsigned int ny,
		       unsigned int nb_ng);

image_t *dupliquer_image(image_t *src);

image_t *dupliquer_image_vers_nmap_anon(image_t *src);

void detruire_image(image_t *p);

void detruire_image_mmap(image_t *img);

image_t *charger_image_pgm(char *nom_fichier);

int sauver_image_pgm(char *nom_fichier, image_t *img);
#endif
