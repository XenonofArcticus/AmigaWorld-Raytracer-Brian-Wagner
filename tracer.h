/**
 * @file tracer.h
 * @brief Raytracer core functions and structures.
 */

#ifndef RAYTRACER_TRACER_H
#define RAYTRACER_TRACER_H

#ifdef WINDOWED_UI
#include <SDL2/SDL.h>
#endif // WINDOWED_UI

#define MAXPOLYS    1500
#define MAXVERTS    1500

#define HUGE        1.0e+12

/**
 * @struct Polygon
 * @brief Represents a polygon surface in 3D space.
 * 
 * This structure defines a polygon used in the raytracer. It includes 
 * the number of vertices, vertex indices, surface normal, and RGB 
 * color components.
 */
struct Polygon
{
   /**
    * @brief Number of vertices in the polygon.
    * 
    * This represents how many vertices make up the polygon.
    */
   int cnt;

   /**
    * @brief Array of vertex indices.
    * 
    * This is a pointer to an array of integers, where each integer 
    * represents the index of a vertex in the vertex array.
    */
   int *vtx;

   /**
    * @brief X component of the surface normal.
    * 
    * The X component of the polygon's normal vector, used for lighting and shading calculations.
    */
   float nx;

   /**
    * @brief Y component of the surface normal.
    * 
    * The Y component of the polygon's normal vector, used for lighting and shading calculations.
    */
   float ny;

   /**
    * @brief Z component of the surface normal.
    * 
    * The Z component of the polygon's normal vector, used for lighting and shading calculations.
    */
   float nz;

   /**
    * @brief Red color component.
    * 
    * The red component of the polygon's color, represented as an integer. Typically in the range [0, 255].
    */
   int r;

   /**
    * @brief Green color component.
    * 
    * The green component of the polygon's color, represented as an integer. Typically in the range [0, 255].
    */
   int g;

   /**
    * @brief Blue color component.
    * 
    * The blue component of the polygon's color, represented as an integer. Typically in the range [0, 255].
    */
   int b;
};

/**
 * @struct Vertex
 * @brief Represents a point in 3D space.
 * 
 * This structure defines a single vertex in 3D space, including its 
 * x, y, and z coordinates.
 */
struct Vertex
{
   float x; ///< X coordinate of the vertex.
   float y; ///< Y coordinate of the vertex.
   float z; ///< Z coordinate of the vertex.
};

/**
 * @struct Ray
 * @brief Represents a ray in 3D space.
 * 
 * This structure defines a ray used in raytracing, which consists of 
 * an origin point (ox, oy, oz) and a direction (dx, dy, dz).
 */
struct Ray
{
   float ox; ///< X coordinate of the ray origin.
   float oy; ///< Y coordinate of the ray origin.
   float oz; ///< Z coordinate of the ray origin.
   
   float dx; ///< X component of the ray direction.
   float dy; ///< Y component of the ray direction.
   float dz; ///< Z component of the ray direction.
};

/**
 * @struct Triangle
 * @brief Represents a triangle in 3D space.
 * 
 * This structure defines a triangle in 3D space, composed of three 
 * vertices, each represented as a triplet of floats, plus a surface normal
 */
struct Triangle
{
   float x1; ///< X coordinate of the first vertex
   float y1; ///< Y coordinate of the first vertex
   float z1; ///< Z coordinate of the first vertex
   float x2; ///< X coordinate of the second vertex
   float y2; ///< Y coordinate of the second vertex
   float z2; ///< Z coordinate of the second vertex
   float x3; ///< X coordinate of the third vertex
   float y3; ///< Y coordinate of the third vertex
   float z3; ///< Z coordinate of the third vertex
   float nx; ///< X coordinate of the surface normal
   float ny; ///< Y coordinate of the surface normal
   float nz; ///< Z coordinate of the surface normal
};

/**
 * @struct Intersection
 * @brief Stores information about a ray-polygon intersection.
 * 
 * This structure holds the details of an intersection between a ray 
 * and a polygon, including the intersection point, distance from the 
 * ray origin, and the polygon that was hit.
 */
struct Intersection
{
   float ix; ///< X coordinate of the intersection point.
   float iy; ///< Y coordinate of the intersection point.
   float iz; ///< Z coordinate of the intersection point.
   
   float dist; ///< Distance from the ray origin to the intersection point.
   
   struct Polygon *poly; ///< Pointer to the polygon that was hit.
};

/**
 * @struct Color
 * @brief Represents an RGB color.
 * 
 * This structure defines a color using red, green, and blue components.
 * Each component is typically in the range [0, 1].
 */
struct Color
{
   float r; ///< Red color component.
   float g; ///< Green color component.
   float b; ///< Blue color component.
};

/**
 * @struct ViewOpts
 * @brief Holds the viewing options for the raytracer.
 * 
 * This structure defines various settings that describe how the scene is viewed
 * and lit. It includes camera position, light source position, the fraction of 
 * the screen to use, view window size, and more. The data is typically loaded 
 * from an external file.
 */
struct ViewOpts
{
   /**
    * @brief X coordinate of the camera (eye) position.
    * 
    * This represents the X position of the camera or eye in 3D space.
    */
   float cax;

   /**
    * @brief Y coordinate of the camera (eye) position.
    * 
    * This represents the Y position of the camera or eye in 3D space.
    */
   float cay;

   /**
    * @brief Z coordinate of the camera (eye) position.
    * 
    * This represents the Z position of the camera or eye in 3D space.
    */
   float caz;

   /**
    * @brief X coordinate of the look-at position.
    * 
    * This represents the X position in 3D space where the camera is looking.
    */
   float lpx;

   /**
    * @brief Y coordinate of the look-at position.
    * 
    * This represents the Y position in 3D space where the camera is looking.
    */
   float lpy;

   /**
    * @brief Z coordinate of the look-at position.
    * 
    * This represents the Z position in 3D space where the camera is looking.
    */
   float lpz;

   /**
    * @brief Fraction of the screen to use.
    * 
    * This value specifies the fraction of the screen to be used for rendering 
    * the scene, typically a value between 0 and 1.0.
    * Currently, a negative value of this will trigger the ground to be textured with
    * a red/yellow gingham checkerboard inspired by the Whitted raytracer paper.
    */
   float scl;

   /**
    * @brief X coordinate of the light source position.
    * 
    * This represents the X position of the light source in 3D space.
    */
   float lsx;

   /**
    * @brief Y coordinate of the light source position.
    * 
    * This represents the Y position of the light source in 3D space.
    */
   float lsy;

   /**
    * @brief Z coordinate of the light source position.
    * 
    * This represents the Z position of the light source in 3D space.
    */
   float lsz;

   /**
    * @brief X coordinate of the view window size.
    * 
    * This represents the horizontal size of the view window (in world units).
    */
   float vpx;

   /**
    * @brief Y coordinate of the view window size.
    * 
    * This represents the vertical size of the view window (in world units).
    */
   float vpy;

   /**
    * @brief Ground position (height).
    * 
    * This represents the vertical position of the ground in the scene, typically
    * a constant or baseline Y position.
    */
   float wdy;

   /**
    * @brief Center of projection distance.
    * 
    * This is the distance from the camera (eye) to the center of projection, 
    * controlling the perspective effect.
    */
   float cpd;
};

#ifdef WINDOWED_UI
int SDL_main(int argc, char *argv[]);
#else // !WINDOWED_UI
int main(int argc, char *argv[]);
#endif // !WINDOWED_UI

#endif // RAYTRACER_TRACER_H

