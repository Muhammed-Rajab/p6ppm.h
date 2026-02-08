#include "p6ppm.h"
#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static inline uint8_t to_u8(double x) {
  if (x < 0.0)
    x = 0.0;
  if (x > 1.0)
    x = 1.0;
  return (uint8_t)(x * 255.999);
}

void shader(double u, double v, double W, double H, double time, double *r,
            double *g, double *b);

int main() {

  PPM image = {.width = 256, .height = 256, .maxval = 255, .data = NULL};

  uint8_t *data = malloc(image.width * image.height * 3);
  image.data = data;

  const size_t FRAME_COUNT = 300;
  const double FPS = 60.0f;

  const size_t W = image.width;
  const size_t H = image.height;

  for (size_t frame = 0; frame < FRAME_COUNT; frame += 1) {

    const double time = frame / FPS;

    for (size_t y = 0; y < H; y += 1) {
      for (size_t x = 0; x < W; x += 1) {

        // uv coordinates
        double u = (double)x / (W - 1);
        double v = (double)y / (H - 1);

        // r, g, b in range [0, 1]
        double r = 0, g = 0, b = 0;

        shader(u, v, W, H, time, &r, &g, &b);

        // color handling
        size_t index = y * W + x;
        data[index * 3 + 0] = to_u8(r);
        data[index * 3 + 1] = to_u8(g);
        data[index * 3 + 2] = to_u8(b);
      }
    }

    size_t image_size = 0;
    uint8_t *image_data = PPM_construct(&image, &image_size);

    fwrite(image_data, 1, image_size, stdout);
    fflush(stdout);

    PPM_free(image_data);
  }

  free(data);

  return 0;
}

void shader(double u, double v, double W, double H, double time, double *r,
            double *g, double *b) {
  // do your shader here
  *r = u;
  *g = v;
  *b = fabs(sin(time));
}
