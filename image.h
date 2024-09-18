/**
 * @file image.h
 * @brief Image manipulation functions used in the raytracer.
 */

#ifndef RAYTRACER_IMAGE_H
#define RAYTRACER_IMAGE_H

#ifdef WINDOWED_UI
#include <SDL2/SDL.h>
#endif // !WINDOWED_UI

#include "tracer.h" // for ViewOpts and Color structures

/**
 * @brief Store color in the RED, GRN, BLU (RGB) buffers.
 *
 * This function stores the provided color in the RGB buffers
 * at the specified x, y position.
 *
 * @param c The RGB color of the pixel.
 * @param x The pixel position to store at.
 * @param y The pixel position to store at.
 */
void storeRGB(struct Color *c, int x, int y);

/**
 * @brief Performs raytracing of a raster image area.
 *
 * This function sends a ray through each pixel of the
 * final image space, computing and storing the resulting color.
 *
 * @param rp The SDL_Renderer used for plotting pixels to the screen to show preview progress.
 */
void traceimage(
#ifdef WINDOWED_UI
   SDL_Renderer * rp
#endif // WINDOWED_UI
   );

/**
 * @brief Interpolates an interim vopts state between a start and end vopts, using a fractional time step
 *
 * For each interpolable parameter in the vopts structure, compute
 * an interim interpolated representation.
 *
 * Parameters interpolated are:
 * [camera] cax, cay, caz
 * [lookposition] lpx, lpy, lpz
 * [lightsource] lsx, lsy, lsz
 * [groundpos] wdy
 * [center of projection] cpd
 *
 * @param dest The vopts to write interpolated state into
 * @param start Starting vopts corresponding to stepfraction=0.0
 * @param end Starting vopts corresponding to stepfraction=1.0
 * @param stepfraction The fractional amount of time elapsed from start to end to interpolate at
 */
void interpolatevopts(struct ViewOpts *dest, struct ViewOpts *start, struct ViewOpts *end, float stepfraction);

#endif // RAYTRACER_IMAGE_H