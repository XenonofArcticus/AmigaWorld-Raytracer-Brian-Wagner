#ifndef INCLUDE_POLYGON_H
#define INCLUDE_POLYGON_H

#define MAXPOLYS    1500
#define MAXVERTS    1500

#define HUGE        1.0e+12

struct Polygon
{
   int   cnt;
   int   *vtx;
   float nx, ny, nz;
   int   r, g, b;
};

struct Vertex
{
   float x, y, z;
};

struct Ray
{
   float ox, oy, oz;
   float dx, dy, dz;
};

struct Triangle
{
   float x1, y1, z1;
   float x2, y2, z2;
   float x3, y3, z3;
   float nx, ny, nz;
};

struct Intersection
{
   float ix, iy, iz;
   float dist;
   void  *poly;
};

struct Color
{
   int r, g, b;
};

struct ViewOpts
{
   float cax, cay, caz;
   float lpx, lpy, lpz;
   float scl;
   float lsx, lsy, lsz;
   float vpx, vpy;
   float wdy;
   float cpd;
};

#endif // INCLUDE_POLYGON_H
