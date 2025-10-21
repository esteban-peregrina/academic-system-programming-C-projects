#include "../inc/binary.h"

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

    
    ui8_t myui8 = read_ui8(myStream);
    fseek(myStream, 0, SEEK_SET); // Attention ! À chaque lecture, le FILE* s'incrémente ! On recommence donc avec fseek().
    printf("uint8: %d\n", myui8);

    ui16_t myui16 = read_ui16(myStream);
    fseek(myStream, 0, SEEK_SET);
    printf("uint16: %d\n", myui16);

    ui32_t myui32 = read_ui32(myStream);
    fseek(myStream, 0, SEEK_SET);
    printf("uint32: %d\n", myui32);

    ui32_t size = file_size(argv[0]);
    printf("Number of bytes in %s: %d\n", argv[0], size);

    if (fclose(myStream) == EOF) {
        fprintf(stderr, "Cannot close file %s !\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    printf("End of program.\n");
    return 0;
}