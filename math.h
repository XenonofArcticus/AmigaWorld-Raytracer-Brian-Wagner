/**
 * @file math.h
 * @brief Mathematical functions for vector and intersection calculations.
 */

#ifndef RAYTRACER_MATH_H
#define RAYTRACER_MATH_H

#include <math.h>
#include "tracer.h" // for Ray, Intersection and Color

extern int   npoly, nvert;
extern int   scrw, scrh;
extern float gnx, gny, gnz;

/**
 * @brief Converts a vector to a unit vector.
 * 
 * This function normalizes a vector by dividing its components by its 
 * length, converting it into a unit vector.
 * 
 * @param x Pointer to the x component of the vector.
 * @param y Pointer to the y component of the vector.
 * @param z Pointer to the z component of the vector.
 */
void unitvector(float *x, float *y, float *z);

/**
 * @brief Transform object vertices and light source position to the view plane coordinate system.
 * 
 * Transform object vertices and light source position to the view plane coordinate system.
 * This is done to greatly simplify calculations.
 */
void transform(void);

/**
 * @brief Calculate surface normals for the polygons.
 * 
 * Calculate surface normals for the polygons by using
 * the vector cross product operation.
 */
void calcnormals(void);

/**
 * @brief Check to see if a ray and triangle intersect.
 * 
 * 
 * @param Ray Pointer to the Ray structure for the ray to be tested.
 * @param t Pointer to the Triangle structure being tested.
 * @param i Pointer to the intersection data if a hit is found.
 * @return int Returns 1 if polygon is hit, 0 if not.
 */
int trianglehit(struct Ray *r, struct Triangle *t, struct Intersection *i);

/**
 * @brief Check to see if a ray and polygon intersect.
 * 
 * Iterates all the vertices of the Polygon and constructs triangles
 * which are then individually submitted to trianglehit for testing.
 * 
 * @param Ray Pointer to the Ray structure for the ray to be tested.
 * @param p Pointer to the Polygon structure for the polygon being tested.
 * @param i Pointer to the intersection data if a hit is found.
 * @return int Returns 1 if polygon is hit, 0 if not.
 */
int polygonhit(struct Ray *r, struct Polygon *p, struct Intersection *i);

/**
 * @brief Check to see if a ray hits the ground.
 * 
 * Compute and report if a candidate ray intersects the ground plane and determine where it hits.
 * 
 * @param Ray Pointer to the Ray structure for the ray to be tested.
 * @param i Pointer to the intersection data if a hit is found.
 * @return int Returns 1 if ground is hit, 0 if not.
 */
int groundhit(struct Ray *r, struct Intersection *i);

/**
 * @brief Checks for shadow intersections.
 * 
 * This function determines if a given intersection point is in shadow 
 * by casting a ray from the point to the light source and checking for 
 * occlusions.
 * 
 * @param i Pointer to the intersection data for the location being tested.
 * @return int Returns 1 if in shadow, 0 if not.
 */
int shadowchk(struct Intersection *i);

/**
 * @brief Shades a point based on its surface properties and light source.
 * 
 * This function calculates the shading at a given intersection point 
 * based on the angle between the surface normal and the light source.
 * 
 * @param i Pointer to the intersection data.
 * @param c Pointer to the color structure where the result is stored.
 */
void shadepoint(struct Intersection *i, struct Color *c);

/**
 * @brief Calculates the sky color based on the ray direction.
 * 
 * This function computes the color of the sky based on the direction of 
 * the ray. It interpolates between a predefined sky and horizon color.
 * 
 * @param r Pointer to the ray structure.
 * @param c Pointer to the color structure where the result is stored.
 */
void shadesky(struct Ray *r, struct Color *c);

#endif // RAYTRACER_MATH_H
