#include "../inc/binary.h"
#include "../inc/img.h"

#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    argc--; // On décompte le nom du fichier
    argv++; // On saute le premier argument
    if (argc == 0) { // Si aucun autre argument n'a été fourni
        printf( "Usage: ./program [filename]\n" );
        exit(EXIT_SUCCESS);
    }

    FILE* myStream = fopen(argv[0], "r");
    if (myStream == NULL) {
        fprintf(stderr, "Cannot open file %s for reading\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    img_t myImage;
    read_header(myStream, &myImage);
    ui32_t size = file_size(argv[0]);
    print_data(&myImage, argv[0], size);

    read_pixels(myStream, &myImage);

    ui32_t* pixel = get_pixel(&myImage, 1, 1);
    printf("Pixel color is: 0x%08X\n", *pixel); // Affichage du premier pixel

    write_ppm("./testfiles/image.ppm", &myImage);

    if (fclose(myStream) == EOF) {
        fprintf(stderr, "Cannot close file %s !\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    printf("End of program.\n");
    return 0;
}