#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#ifndef BITMAP_H
#define BITMAP_H
typedef enum {
  BMP_OK,
  BMP_NO_FILE,
  BMP_NO_STRUCT,
  BMP_INVALID_SIGNATURE
} bitmap_status;

typedef struct {
  uint32_t size;
  uint32_t reserved;
  uint32_t offset;
  
  uint32_t header_size;
  int32_t  width;
  int32_t  height;
  uint8_t  planes;
  uint16_t bits_per_pixel;
  uint16_t compression;
  uint32_t image_size;
  uint32_t pixels_per_meter_x;
  uint32_t pixels_per_meter_y;
  uint32_t colors_table_count; // may can be uint16_t;
  uint32_t colors_count; // may can be uint16_t;

  uint8_t *data;
} bitmap_t;

bitmap_status bitmap_decode(bitmap_t *b, FILE *f);
bitmap_status bitmap_free(bitmap_t *b, FILE *f);
#endif
