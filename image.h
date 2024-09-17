#ifndef RAYTRACER_IMAGE_H
#define RAYTRACER_IMAGE_H

#ifdef WINDOWED_UI
#include <SDL2/SDL.h>
#endif // !WINDOWED_UI

#include "tracer.h" // for ViewOpts and Color structures

/* Store color in the RED, GRN, BLU (RGB) buffers. */

void storeRGB(struct Color *c, int x, int y);

void traceimage(
#ifdef WINDOWED_UI
   SDL_Renderer * rp
#endif // WINDOWED_UI
   );

void interpolatevopts(struct ViewOpts *dest, struct ViewOpts *start, struct ViewOpts *end, float stepfraction);

#endif // RAYTRACER_IMAGE_H