#ifndef P6PPM_H
#define P6PPM_H

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * p6ppm.h: graphics, minus the graphics API
 *
 * i wrote this when i started learning C because I was interested in graphics
 * programming and didn't want to learn heavy libraries or graphics APIs. It
 * turns out that to draw an image, all you need is a  WIDTH x HEIGHT x 3 array
 * holding raw R, G, and B values.
 *
 * this header provides a minimal helper for constructing binary PPM(P6) image
 * from such an array.
 *
 * NOTE: the error handling here is intentionally simple. it's not perfect, but
 * it's good enough for small experiments and learning projects.
 *
 * struct PPM
 *  - width : image width in pixels
 *  - height: image height in pixels
 *  - maxval: maximum per-channel value (usually 255)
 *  - data  : WIDTH x HEIGHT x 3 array of RGB pixel data
 *
 * lifecycle:
 * 1. define WIDTH, HEIGHT, MAX_VAL
 * 2. allocate a WIDTH x HEIGHT x 3 byte array
 * 3. write RGB values into the array
 * 4. populate a PPM struct
 * 5. call PPM_construct() to generate the P6 image buffer
 * 6. free the buffer returned by PPM_construct()
 * 7. free the original pixel array if it was heap-allocated
 * */
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
    fprintf(stderr, "ppm header too large\n");
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
