#ifndef P6PPM_H
#define P6PPM_H

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  size_t width;
  size_t height;
  size_t maxval;
  uint8_t *data;
} PPM;

// constructs a PPM P6 image
static inline uint8_t *PPM_construct(const PPM *ppm, size_t *image_size) {
  // sanity check #1
  if (!ppm || !image_size) {
    fprintf(stderr, "ppm or image_size is NULL\n");
    return NULL;
  }

  // sanity check #2
  if (!ppm->data) {
    fprintf(stderr, "ppm->data is NULL\n");
    return NULL;
  }

  // sanity check #3
  if (ppm->height > SIZE_MAX / 3 || ppm->width > SIZE_MAX / (ppm->height * 3)) {
    fprintf(stderr, "image too large\n");
    return NULL;
  }

  // sanity check #4
  if (ppm->maxval == 0 || ppm->maxval > 255) {
    fprintf(stderr, "invalid maxval (must be 1–255)\n");
    return NULL;
  }

  /* format
   * ------
   *  P6
   *  <WIDTH> <HEIGHT>
   *  <MAXVAL>
   *  <BINARY RGBRGBRGB...>
   */
  char header[128];

  int header_len = snprintf(header, sizeof(header), "P6\n%zu %zu\n%zu\n",
                            ppm->width, ppm->height, ppm->maxval);
  if (header_len <= 0 || (size_t)header_len >= sizeof(header)) {
    fprintf(stderr, "PPM header too large\n");
    return NULL;
  }

  size_t pixel_bytes = ppm->width * ppm->height * 3;
  *image_size = header_len + pixel_bytes;

  // alloc buf
  uint8_t *buf = (uint8_t *)malloc(*image_size);
  if (!buf) {
    perror("malloc");
    return NULL;
  }

  // copy header to buf
  memcpy(buf, header, header_len);

  // copy pixel data to buf
  memcpy(buf + header_len, ppm->data, pixel_bytes);

  return buf;
}

static inline void PPM_free(uint8_t *constructed_image) {
  if (!constructed_image)
    return;
  free(constructed_image);
}

#endif
