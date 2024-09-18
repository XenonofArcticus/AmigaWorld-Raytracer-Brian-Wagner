/**
 * @file load.h
 * @brief Functions to load scene data for the raytracer.
 */
#ifndef RAYTRACER_LOAD_H
#define RAYTRACER_LOAD_H

#include "tracer.h"

/**
 * @brief Convert GEO color code to equivalent RGB color values.
 *
 * Convert from an int material color to an RGB triplet.
 * This function uses a lookup table to map ordinal material numbers
 * into a RGB values and store them in the Polygon
 *
 * @param col The material ordinal of the polygon.
 * @param poly The polygon to store the RGB value into.
 */
void convertcol(int col, struct Polygon *poly);

/**
 * @brief Load GEO object and store in the previously allocated buffers.
 *
 * Load GEO object and store in the previously allocated buffers.
 * Vertex arrays are allocated during the load since the number of
 * vertices per polygon can vary greatly.
 *
 * @param file The GEO format filename
 * @param loadverts The array of Vertex objects to load into
 */
int loadobject(char *file, struct Vertex *loadverts);

/**
 * @brief Load viewing options and store into a ViewOpts structure.
 *
 * Loads and parses the textual viewopts format into the ViewOpts struture
 *
 * @param file The viewopts filename
 * @param loadverts The destination ViewOpts to load into
 */
int loadvopts(char *file, struct ViewOpts *destvopts);

#endif // RAYTRACER_LOAD_H
