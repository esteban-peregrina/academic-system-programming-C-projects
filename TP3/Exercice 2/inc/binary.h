#ifndef BINARY_H
#define BINARY_H

#include <stdint.h>
#include <stdio.h>

// J'ai pas bien compris l'intérêt mais pourquoi pas
typedef uint8_t ui8_t;
typedef uint16_t ui16_t;
typedef uint32_t ui32_t;

ui8_t read_ui8(FILE* f);
ui16_t read_ui16(FILE* f);
ui32_t read_ui32(FILE* f);

ui32_t file_size(const char* fname);

#endif // BINARY_H