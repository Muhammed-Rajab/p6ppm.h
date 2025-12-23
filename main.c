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

uint8_t *PPM_construct(const PPM *ppm, size_t *image_size) {
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
  if (ppm->width > SIZE_MAX / (ppm->height * 3)) {
    fprintf(stderr, "image too large\n");
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
  uint8_t *buf = malloc(*image_size);
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

int main() {

  PPM image = {.width = 256, .height = 256, .maxval = 255, .data = NULL};

  uint8_t *data = malloc(image.width * image.height * 3);
  image.data = data;

  for (size_t y = 0; y < image.height; y += 1) {
    for (size_t x = 0; x < image.width; x += 1) {
      size_t index = y * image.width + x;

      uint8_t r = (uint8_t)(255 * ((float)(x) / (float)(image.width)));
      uint8_t g = (uint8_t)(255 * ((float)(y) / (float)(image.height)));
      uint8_t b = (uint8_t)(255 * ((float)(x + y) /
                                   (float)(image.width + image.height)));

      data[index * 3 + 0] = r;
      data[index * 3 + 1] = g;
      data[index * 3 + 2] = b;
    }
  }

  size_t image_size = 0;
  uint8_t *image_data = PPM_construct(&image, &image_size);

  fwrite(image_data, 1, image_size, stdout);
  fflush(stdout);

  free(data);
  free(image_data);

  return 0;
}
