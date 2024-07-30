#ifndef INCLUDE_POLYGON_H
#define INCLUDE_POLYGON_H

#define MAXPOLYS  1500
#define MAXVERTS  1500

#define HUGE 1.0e+12

struct Polygon {
   SHORT cnt;
   SHORT *vtx;
   FLOAT nx, ny, nz;
   SHORT r, g, b;
};

struct Vertex {
   FLOAT x, y, z;
};

struct Ray {
   FLOAT ox, oy, oz;
   FLOAT dx, dy, dz;
};

struct Triangle {
   FLOAT x1, y1, z1;
   FLOAT x2, y2, z2;
   FLOAT x3, y3, z3;
   FLOAT nx, ny, nz;
};

struct Intersection {
   FLOAT ix, iy, iz;
   FLOAT dist;
   VOID *poly;
};

struct Color {
   SHORT r, g, b;
};

struct ViewOpts {
   FLOAT cax, cay, caz;
   FLOAT lpx, lpy, lpz;
   FLOAT scl;
   FLOAT lsx, lsy, lsz;
   FLOAT vpx, vpy;
   FLOAT wdy;
   FLOAT cpd;
};

#endif // INCLUDE_POLYGON_H
