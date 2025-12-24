# p6ppm.h

A tiny, self-contained **single-header C utility** for constructing
**binary PPM (P6)** images entirely in memory.

You can copy the `p6ppm.h` file to your project and use it as you wish.

## Project Structure

```
.
├── example.c   # Example usage (animated PPM stream)
├── p6ppm.h     # Minimal PPM (P6) construction utilities
└── Makefile    # Build + video generation
```

## API Summary

- `PPM_construct()` — constructs a complete P6 PPM image in memory
- `PPM_free()` — frees a buffer returned by `PPM_construct()`

## Minimal Usage Example

The following example shows how to use `p6ppm.h` to construct
a single P6 PPM image entirely in memory and write it to a file.

```c
#include "p6ppm.h"
#include <stdio.h>

int main(void) {
    const size_t W = 256;
    const size_t H = 256;

    uint8_t pixels[W * H * 3];

    // Fill image with a simple gradient
    for (size_t y = 0; y < H; ++y) {
        for (size_t x = 0; x < W; ++x) {
            size_t i = (y * W + x) * 3;
            pixels[i + 0] = (uint8_t)(x % 256); // R
            pixels[i + 1] = (uint8_t)(y % 256); // G
            pixels[i + 2] = 0;                  // B
        }
    }

    PPM image = {
        .width  = W,
        .height = H,
        .maxval = 255,
        .data   = pixels
    };

    size_t image_size;
    uint8_t *ppm = PPM_construct(&image, &image_size);

    if (!ppm)
        return 1;

    FILE *f = fopen("output.ppm", "wb");
    fwrite(ppm, 1, image_size, f);
    fclose(f);

    PPM_free(ppm);
    return 0;
}
```

## Requirements

### Library
- Any ISO C compiler

### Example
- gcc (or compatible C compiler)
- ffmpeg (required to generate the video)

## Build

Compile the example program:

```sh
make
```
This produces the binary:

```sh
build/example
```

## Run and Generate Video

To render frames and encode them into a video:

```sh
make video
```

This will:
- Build the example
- Run it
- Pipe raw PPM frames to ffmpeg
- Produce example.mp4

## How It Works

1. Each frame is rendered in memory as RGB data
2. The buffer is encoded as a P6 PPM buffer
3. The image is written to `stdout`
4. `ffmpeg` reads the stream using `image2pipe`

Conceptually:

```sh
build/example | ffmpeg -f image2pipe -framerate 60 -i - example.mp4
```
## Shader Logic

The function:
```c
void shader(double u, double v, double W, double H,
            double time, double *r, double *g, double *b);
```
acts like a simple fragment shader:

- `u`, `v` are normalized coordinates
- `time` advances per frame
- `r`, `g`, `b` must be written in the range `[0, 1]`

Modify this function to change the animation.

## Memory Ownership
`PPM_construct()` allocates a new buffer. The caller must free it using `PPM_free()`.

Pixel data (`ppm.data`) is owned by the caller

## Cleaning Up

Remove build artifacts and generated video:
```sh
make clean
```

## Notes

This library avoids file I/O and operates purely on memory buffers.
It is intentionally minimal, portable, and suitable for streaming
pipelines or small software rendering experiments.

If you understand your math, you do not need a graphics API to render interesting images.
