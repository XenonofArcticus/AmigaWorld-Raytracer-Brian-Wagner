/**
 * @file free.h
 * @brief Functions to manage memory deallocation for raytracer structures.
 */

#ifndef RAYTRACER_FREE_H
#define RAYTRACER_FREE_H

/**
 * @brief Frees the memory allocated for polygons' vertices.
 *
 * Free all polygon vertex arrays which are allocated during the object
 * load function.
 */

void freevtxarrays();
#endif // RAYTRACER_FREE_H
