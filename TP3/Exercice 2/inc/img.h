#ifndef IMAGE_H
#define IMAGE_H

#include "binary.h"

#include <stdio.h>

typedef struct {
    ui8_t id_length;
    ui8_t colour_map_type;
    ui8_t image_type;
    ui16_t colour_map_origin;
    ui16_t colour_map_length;
    ui8_t colour_map_depth;
    ui16_t x_origin;
    ui16_t y_origin;
    ui16_t width;
    ui16_t height;
    ui8_t bbp;
    ui8_t image_descriptor;
    ui32_t* pixels; 
} img_t;

void read_header(FILE* f, img_t* img);
void print_data(img_t* img, char* filename, int filesize);
ui32_t* get_pixel(const img_t* img, int row, int col);
ui32_t make_pixel(ui8_t r, ui8_t g, ui8_t b);
void read_pixels(FILE* f, img_t* img);
void write_ppm(const char* filename, const img_t* img);

#endif // IMAGE_H