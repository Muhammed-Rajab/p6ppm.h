#include "p6ppm.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
  PPM_free(image_data);

  return 0;
}
