#include "bitmap.h"

/*void bitmap_encode(bitmap_t *b, char *out) {*/

/*}*/

bitmap_status bitmap_decode(bitmap_t *b, FILE *f) {
  if (b == NULL)
    return BMP_NO_STRUCT;

  if (f ==  NULL)
    return BMP_NO_FILE;

  if (fgetc(f) != 'B' || fgetc(f) != 'M')
    return BMP_INVALID_SIGNATURE;

  // reading the header, removing the 'data' member to ensure correct alignment;
  fread(b, sizeof(bitmap_t) - 8, 1, f);

  // get pixel_array data;
  uint32_t row_size = (b->bits_per_pixel * b->width + 31) / 32 * 4;
  fseek(f, b->offset, SEEK_SET);
  b->data = (uint8_t *) malloc(row_size * b->height);
  fread(b->data, b->bits_per_pixel / 8, b->width * b->height, f);

  return BMP_OK;
}

bitmap_status bitmap_free(bitmap_t *b, FILE *f) {
  if (b->data != NULL)
    free(b->data);

  if (f != NULL)
    fclose(f);

  return BMP_OK;
}
