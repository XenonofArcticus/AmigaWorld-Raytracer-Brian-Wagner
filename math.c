#include <math.h> // system math.h
#include "types.h"
//#include "functions.h"

#include "tracer.h"
#include "math.h" // project's math.h

extern struct Polygon *polys;

extern struct Vertex *verts;

extern struct ViewOpts vopts;

extern SHORT npoly, nvert;

extern SHORT scrw, scrh;

extern FLOAT gnx, gny, gnz;

// extern FLOAT sqrt();// defined in system math.h

/* Calculate a unit vector by dividing each component by the their */
/* combined length. */

VOID unitvector(x, y, z)
FLOAT *x, *y, *z;
{
   FLOAT len;

   len = sqrt(*x * *x + *y * *y + *z * *z);

   if (len > 0.0) {
      *x /= len;
      *y /= len;
      *z /= len;
   }

   return;
}

/* Transform object vertices and light source position to the view plane */
/* coordinate system.  This is done to greatly simplify calculations. */

VOID transform()
{
   LONG i;
   FLOAT nx, ny, nz, s;
   FLOAT sx, cx, sy, cy;
   FLOAT vx, vy, vz;
   FLOAT tx, ty, tz;

   nx = vopts.lpx - vopts.cax;
   ny = vopts.lpy - vopts.cay;
   nz = vopts.lpz - vopts.caz;

   unitvector(&nx, &ny, &nz);

   s = sqrt(ny * ny + nz * nz);

   if (s > 0.0) {
      sx = -ny / s;
      cx = -nz / s;
   }

   sy = -nx;
   cy = s;

   for (i = 0; i < nvert; i++) {
      vx = verts[i].x - vopts.cax;
      vy = verts[i].y - vopts.cay;
      vz = verts[i].z - vopts.caz;

      if (s > 0.0) {
         ty = vy * cx - vz * sx;
         tz = vy * sx + vz * cx;

         vy = ty;
         vz = tz;
      }

      tx = vx * cy - vz * sy;
      tz = vx * sy + vz * cy;

      vx = tx;
      vz = tz;

      verts[i].x = vx;
      verts[i].y = vy;
      verts[i].z = vz;

      continue;
   }

   vx = vopts.lsx - vopts.cax;
   vy = vopts.lsy - vopts.cay;
   vz = vopts.lsz - vopts.caz;

   if (s > 0.0) {
      ty = vy * cx - vz * sx;
      tz = vy * sx + vz * cx;

      vy = ty;
      vz = tz;
   }

   tx = vx * cy - vz * sy;
   tz = vx * sy + vz * cy;

   vx = tx;
   vz = tz;

   vopts.lsx = vx;
   vopts.lsy = vy;
   vopts.lsz = vz;

   /* Also, transform the "ground" normal. */

   vx = gnx;
   vy = gny;
   vz = gnz;

   if (s > 0.0) {
      ty = vy * cx - vz * sx;
      tz = vy * sx + vz * cx;

      vy = ty;
      vz = tz;
   }

   tx = vx * cy - vz * sy;
   tz = vx * sy + vz * cy;

   vx = tx;
   vz = tz;

   gnx = vx;
   gny = vy;
   gnz = vz;

   return;
}

/* Calculate surface normals for the polygons by using the vector cross */
/* product operation. */

VOID calcnormals()
{
   LONG i, oi, ai, bi;
   FLOAT ax, ay, az, bx, by, bz;

   for (i = 0; i < npoly; i++) {
      oi = polys[i].vtx[0];

      ai = polys[i].vtx[1];

      ax = verts[ai].x - verts[oi].x;
      ay = verts[ai].y - verts[oi].y;
      az = verts[ai].z - verts[oi].z;

      unitvector(&ax, &ay, &az);

      bi = polys[i].vtx[polys[i].cnt - 1];

      bx = verts[bi].x - verts[oi].x;
      by = verts[bi].y - verts[oi].y;
      bz = verts[bi].z - verts[oi].z;

      unitvector(&bx, &by, &bz);

      polys[i].nx = -(ay * bz - by * az);
      polys[i].ny = -(az * bx - bz * ax);
      polys[i].nz = -(ax * by - bx * ay);

      continue;
   }

   return;
}

/* Check to see if a ray and triangle intersect. */

SHORT trianglehit(r, t, i)
struct Ray *r;
struct Triangle *t;
struct Intersection *i;
{
   FLOAT px, py, pz, n, d, v;
   FLOAT ix, iy, iz, x, y;
   FLOAT x21, y21, z21, x31, y31, z31;
   FLOAT nxx, nxy, nxz, nyx, nyy, nyz;
   FLOAT dxy, dyx, nx, ny, nz;

   nx = t->nx;
   ny = t->ny;
   nz = t->nz;

   px = r->ox - t->x1;
   py = r->oy - t->y1;
   pz = r->oz - t->z1;

   n = px * nx + py * ny + pz * nz;

   d = r->dx * nx + r->dy * ny + r->dz * nz;

   if (d == 0.0) return(0);

   v = -n / d;

   if (v <= 0.0 || v > i->dist) return(0);

   ix = (v * r->dx) + r->ox;
   iy = (v * r->dy) + r->oy;
   iz = (v * r->dz) + r->oz;

   px = ix - t->x1;
   py = iy - t->y1;
   pz = iz - t->z1;

   x21 = t->x2 - t->x1;
   y21 = t->y2 - t->y1;
   z21 = t->z2 - t->z1;

   x31 = t->x3 - t->x1;
   y31 = t->y3 - t->y1;
   z31 = t->z3 - t->z1;

   nxx = ny * z21 - nz * y21;
   nxy = nz * x21 - nx * z21;
   nxz = nx * y21 - ny * x21;

   nyx = ny * z31 - nz * y31;
   nyy = nz * x31 - nx * z31;
   nyz = nx * y31 - ny * x31;

   dxy = 1.0 / (x21 * nyx + y21 * nyy + z21 * nyz);

   dyx = 1.0 / (x31 * nxx + y31 * nxy + z31 * nxz);

   n = px * nyx + py * nyy + pz * nyz;

   x = n * dxy;

   if (x <= 0.0) return(0);

   n = px * nxx + py * nxy + pz * nxz;

   y = n * dyx;

   if (y <= 0.0) return(0);

   if (x + y > 1.0) return(0);

   i->ix = ix;
   i->iy = iy;
   i->iz = iz;

   i->dist = v;

   return(1);
}

/* Check to see if a ray and polygon intersect. */

SHORT polygonhit(r, p, i)
struct Ray *r;
struct Polygon *p;
struct Intersection *i;
{
   struct Triangle t;

   SHORT nt, l, v, hit;

   nt = p->cnt - 2;

   for (l = 0; l < nt; l++) {
      v = p->vtx[0];

      t.x1 = verts[v].x;
      t.y1 = verts[v].y;
      t.z1 = verts[v].z;

      v = p->vtx[l + 1];

      t.x2 = verts[v].x;
      t.y2 = verts[v].y;
      t.z2 = verts[v].z;

      v = p->vtx[l + 2];

      t.x3 = verts[v].x;
      t.y3 = verts[v].y;
      t.z3 = verts[v].z;

      t.nx = p->nx;
      t.ny = p->ny;
      t.nz = p->nz;

      hit = trianglehit(r, &t, i);

      if (hit) {
         i->poly = p;

         return(1);
      }
   }

   return(0);
}

/* Check to see if a ray hits the ground. */

SHORT groundhit(r, i)
struct Ray *r;
struct Intersection *i;
{
   FLOAT px, py, pz;
   FLOAT n, d, v;

   d = r->dx * gnx + r->dy * gny + r->dz * gnz;

   if (d >= 0.0) return(0);

   px = r->ox - 0.0;
   py = r->oy - vopts.wdy;
   pz = r->oz - 0.0;

   n = px * gnx + py * gny + pz * gnz;

   if (d == 0.0) return(0);

   v = -n / d;

   if (v <= 0.0 || v > i->dist) return(0);

   i->ix = (v * r->dx) + r->ox;
   i->iy = (v * r->dy) + r->oy;
   i->iz = (v * r->dz) + r->oz;

   i->dist = v;

   return(1);
}

/* Check to see if ANY polygons lie between a surface point and */
/* the light source. */

SHORT shadowchk(i)
struct Intersection *i;
{
   struct Ray ray;

   struct Intersection si;

   SHORT l;
   FLOAT dx, dy, dz;

   dx = vopts.lsx - i->ix;
   dy = vopts.lsy - i->iy;
   dz = vopts.lsz - i->iz;

   unitvector(&dx, &dy, &dz);

   ray.ox = i->ix + (0.1 * dx);
   ray.oy = i->iy + (0.1 * dy);
   ray.oz = i->iz + (0.1 * dz);

   ray.dx = dx;
   ray.dy = dy;
   ray.dz = dz;

   si.dist = HUGE;

   for (l = 0; l < npoly; l++) {
      if (polygonhit(&ray, &polys[l], &si)) return(1);

      continue;
   }

   return(0);
}

/* Calculate shade of color for a given surface point. */

VOID shadepoint(i, c)
struct Intersection *i;
struct Color *c;
{
   struct Polygon *p;

   FLOAT lx, ly, lz, dp;

   p = i->poly;

   lx = i->ix - vopts.lsx;
   ly = i->iy - vopts.lsy;
   lz = i->iz - vopts.lsz;

   unitvector(&lx, &ly, &lz);

   dp = -lx * p->nx + -ly * p->ny + -lz * p->nz;

   if (dp < 0.0) dp = 0.0;

   if (dp > 0.0 && shadowchk(i)) dp *= 0.2;

   c->r = p->r * dp;
   c->g = p->g * dp;
   c->b = p->b * dp;

   return;
}

/* Calculate a sky color based on ray direction. */

VOID shadesky(r, c)
struct Ray *r;
struct Color *c;
{
   SHORT zr, zg, zb;
   SHORT hr, hg, hb;
   FLOAT dp;

   dp = r->dx * gnx + r->dy * gny + r->dz * gnz;

   if (dp < 0.0) return;

   zr = 0;
   zg = 0;
   zb = 100;

   hr = 0;
   hg = 40;
   hb = 160;

   c->r = hr + (zr - hr) * dp;
   c->g = hg + (zg - hg) * dp;
   c->b = hb + (zb - hb) * dp;

   return;
}

