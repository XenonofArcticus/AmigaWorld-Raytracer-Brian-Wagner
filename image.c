#ifdef WINDOWED_UI
#include <SDL2/SDL.h>
#endif // WINDOWED_UI

#include "tracer.h"
#include "math.h"
#ifdef _WIN32
#include <Windows.h>
#endif // _WIN32

extern struct Polygon *polys;

extern struct ViewOpts vopts;

extern unsigned char *red, *grn, *blu;

extern int npoly;

extern int scrw, scrh;

/* Store color in the RED, GRN, BLU (RGB) buffers. */

void storeRGB(c, x, y)
struct Color *c;
int x, y;
{
   int pos;

   pos = y * scrw + x;

   red[pos] = c->r;
   grn[pos] = c->g;
   blu[pos] = c->b;

   return;
}

/* Send a ray through each pixel of final image. */

void traceimage(
#ifdef WINDOWED_UI
   rp
#endif // WINDOWED_UI
   )
#ifdef WINDOWED_UI
SDL_Renderer *rp;
#endif // WINDOWED_UI
{
#ifdef WINDOWED_UI
SDL_Event event;
#endif // WINDOWED_UI
   struct Ray ray;

   struct Intersection isec;

   struct Color color;

   int i, j, k, l;
   int actw, acth;
   float vpx, vpy;
   float px, py, ar;
   float dx, dy, dz;

   /* Calculate aspect ratio value. */

   ar = (scrw * ((scrh * 4.0) / (scrw * 3.0))) / scrh;

   /* Squeeze width of viewport with aspect ratio value. */

   vpx = vopts.vpx * ar;
   vpy = vopts.vpy;

   /* Calculate actual portion of the screen to use. */

   actw = scrw * vopts.scl;
   acth = scrh * vopts.scl;
#ifdef WINDOWED_UI
   SDL_SetRenderDrawColor(rp, 0, 0, 0, 0);
#endif // WINDOWED_UI
   for (i = 0; i < actw; i++) {
      for (j = 0; j < acth; j++) {
         px = (float)i + 0.5;
         px = vpx * ((px / actw) - 0.5);

         py = (float)(acth - 1 - j) - 0.5;
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
#ifdef WINDOWED_UI 
             SDL_SetRenderDrawColor(rp, (Uint8)color.r, (Uint8)color.g, (Uint8)color.b, 255);
             SDL_RenderDrawPoint(rp, i, j);
             // present graphics and run event loop
             static unsigned int PresentInterval;
             // only do the work every 6400 pixels traced, to minimize overhead
             if(!((++PresentInterval + 1) % 6400)) {
               SDL_RenderPresent(rp);
               if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
                  return; // kind of an abrupt exit, but the code is simple enough to do it this way
             }
#endif // WINDOWED_UI 
         continue;
      }
   }

   return;
}

