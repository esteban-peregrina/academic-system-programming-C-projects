#include "../inc/binary.h"

#include <stdio.h>
#include <stdlib.h>

ui8_t read_ui8(FILE* f) {
    ui8_t myui8;

    printf("Reading ui8 byte...\n");
    if (fread(&myui8, 1, 1, f) != 1) { // On lit un seul octet
        fprintf(stderr, "Cannot read ui8 byte in file.\n");
        exit(EXIT_FAILURE);
    }

    return myui8;
}

ui16_t read_ui16(FILE* f) {
    ui16_t myui16;

    printf("Reading ui16 bytes...\n");
    if (fread(&myui16, 1, 2, f) != 2) { // On lit deux octets
        fprintf(stderr, "Cannot read ui16 bytes in file.\n");
        exit(EXIT_FAILURE);
    }

    return myui16;
}

ui32_t read_ui32(FILE* f) {
    ui32_t myui32;

    printf("Reading ui32 bytes...\n");
    if (fread(&myui32, 1, 4, f) != 4) { // On lit quatre octets
        fprintf(stderr, "Cannot read ui32 bytes in file.\n");
        exit(EXIT_FAILURE);
    }

    return myui32;
}

ui32_t file_size(const char* fname) {
    FILE* myStream = fopen(fname, "r");
    if (myStream == NULL) {
        fprintf(stderr, "Cannot open file %s for reading\n", fname);
        exit(EXIT_FAILURE);
    }
    fseek(myStream, 0, SEEK_END);
    return ftell(myStream); // ftell() renvoie directement la position correspondant au nombre d'octets depuis le début du fichier
}