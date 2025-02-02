#include "exec/types.h"
#include "intuition/intuition.h"
#include "functions.h"

#include "tracer.h"

extern struct Polygon *polys;

extern struct ViewOpts vopts;

extern UBYTE *red, *grn, *blu;

extern SHORT npoly;

extern SHORT scrw, scrh;

/* Store color in the RED, GRN, BLU (RGB) buffers. */

VOID storeRGB(c, x, y)
struct Color *c;
SHORT x, y;
{
   LONG pos;

   pos = (LONG)y * scrw + x;

   red[pos] = c->r;
   grn[pos] = c->g;
   blu[pos] = c->b;

   return;
}

/* Send a ray through each pixel of final image. */

VOID traceimage(rp)
struct RastPort *rp;
{
   struct Ray ray;

   struct Intersection isec;

   struct Color color;

   SHORT i, j, k, l;
   SHORT actw, acth;
   FLOAT vpx, vpy;
   FLOAT px, py, ar;
   FLOAT dx, dy, dz;

   /* Calculate aspect ratio value. */

   ar = (scrw * ((scrh * 4.0) / (scrw * 3.0))) / scrh;

   /* Squeeze width of viewport with aspect ratio value. */

   vpx = vopts.vpx * ar;
   vpy = vopts.vpy;

   /* Calculate actual portion of the screen to use. */

   actw = scrw * vopts.scl;
   acth = scrh * vopts.scl;

   SetAPen(rp, (LONG)1);

   for (i = 0; i < actw; i++) {
      for (j = 0; j < acth; j++) {
         px = (FLOAT)i + 0.5;
         px = vpx * ((px / actw) - 0.5);

         py = (FLOAT)(acth - 1 - j) - 0.5;
         py = vpy * ((py / acth) - 0.5);

         ray.ox = 0.0;
         ray.oy = 0.0;
         ray.oz = vopts.cpd;

         dx = px - ray.ox;
         dy = py - ray.oy;
         dz = 0.0 - ray.oz;

         unitvector(&dx, &dy, &dz);

         ray.dx = dx;
         ray.dy = dy;
         ray.dz = dz;

         isec.dist = HUGE;

         l = 0;

         for (k = 0; k < npoly; k++) {
            if (polygonhit(&ray, &polys[k], &isec)) l = 1;

            continue;
         }

         if (groundhit(&ray, &isec)) {
            if (shadowchk(&isec)) {
               color.r = 27;
               color.g = 14;
               color.b = 0;
            }
            else {
               color.r = 136;
               color.g = 68;
               color.b = 0;
            }

            storeRGB(&color, i, j);

            l = -1;
         }

         if (l == 1) {
            shadepoint(&isec, &color);

            storeRGB(&color, i, j);
         } else

         if (l == 0) {
            shadesky(&ray, &color);

            storeRGB(&color, i, j);
         }

         WritePixel(rp, (LONG)i, (LONG)j);

         continue;
      }
   }

   return;
}

