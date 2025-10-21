#include "../inc/img.h"
#include "../inc/binary.h"

#include <stdlib.h>

void read_header(FILE* f, img_t* img) {
    printf("Reading header...\n");
    img->id_length = read_ui8(f);
    img->colour_map_type = read_ui8(f);
    img->image_type = read_ui8(f);
    img->colour_map_origin = read_ui16(f);
    img->colour_map_length = read_ui16(f);
    img->colour_map_depth = read_ui8(f);
    img->x_origin = read_ui16(f);
    img->y_origin = read_ui16(f);
    img->width = read_ui16(f);
    img->height = read_ui16(f);
    img->bbp = read_ui8(f);
    img->image_descriptor = read_ui8(f);
}

void print_data(img_t* img, char* filename, int filesize) {
    printf("Image name is: %s\n", filename);
    printf("Image size is: %d\n", filesize);
    printf("Image size is: %d x %d\n", img->width, img->height);
    printf("Image bits by pixels are: %d\n", img->bbp);
}

ui32_t* get_pixel(const img_t* img, int row, int col) {
    /*
    Présuppose que l'indexation commence en (1,1)
    */
    return &img->pixels[img->width * (img->height - row) + (col - 1)];
}

ui32_t make_pixel(ui8_t r, ui8_t g, ui8_t b) {
    return (0x00 << 24) | (b << 16) | (g << 8) | (r); // Le MSB est 0000
}

void read_pixels(FILE* f, img_t* img) {
    fseek(f, img->id_length + img->colour_map_type * img->colour_map_length, SEEK_CUR);
    int res = img->width * img->height;
    ui32_t* pixels_tab = malloc(sizeof(ui32_t) * res);
    for (int p = 0; p < res; p++) {
        ui8_t b = read_ui8(f);
        ui8_t g = read_ui8(f);
        ui8_t r = read_ui8(f);
        pixels_tab[p] = make_pixel(r, g, b);
    }
    img->pixels = pixels_tab;
}

void write_ppm(const char* filename, const img_t* img) {
    FILE* myStream = fopen(filename, "w");
    if (myStream == NULL) {
        fprintf(stderr, "Cannot open file %s for writing\n", filename);
        exit(EXIT_FAILURE);
    }

    fprintf(myStream, "P6\n%d %d\n255\n", img->width, img->height);
    
    for (int y = img->height - 1; y >= 0; y--) {
        for (int x = 0; x < img->width; x++) {
            ui32_t pixel = img->pixels[y * img->width + x];
            ui8_t r = pixel & 0xFF;
            ui8_t g = (pixel >> 8) & 0xFF;
            ui8_t b = (pixel >> 16) & 0xFF;
            fputc(r, myStream);
            fputc(g, myStream);
            fputc(b, myStream);
        }
    }

    if (fclose(myStream) == EOF) {
        fprintf(stderr, "Cannot close file %s !\n", filename);
        exit(EXIT_FAILURE);
    }
}

