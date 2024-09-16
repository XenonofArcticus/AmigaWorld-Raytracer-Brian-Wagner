#ifdef WINDOWED_UI
#include <SDL2/SDL.h>
#endif // WINDOWED_UI

#include "tracer.h"
#include "math.h"
#ifdef _WIN32
#include <Windows.h>
#endif // _WIN32

extern struct Polygon  *polys;

extern struct ViewOpts vopts;

extern unsigned char   *red, *grn, *blu;

extern int             npoly;

extern int             scrw, scrh;

/* Store color in the RED, GRN, BLU (RGB) buffers. */


void storeRGB(struct Color *  c,
              int x,
              int y)
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
   SDL_Renderer *rp
#else // WINDOWED_UI
   void
#endif // WINDOWED_UI
   )
{
#ifdef WINDOWED_UI
   SDL_Event           event;
#endif // WINDOWED_UI
   struct Ray          ray;

   struct Intersection isec;

   struct Color        color;

   int                 i, j, k, l;
   int                 actw, acth;
   float               vpx, vpy;
   float               px, py, ar;
   float               dx, dy, dz;

   int                 Gingham = 0;
   int                 GroundR, GroundG, GroundB;
   float               ShadeR, ShadeG, ShadeB;
   int                 WhittedGroundA_R, WhittedGroundA_G, WhittedGroundA_B;
   int                 WhittedGroundB_R, WhittedGroundB_G, WhittedGroundB_B;
   int                 WhittedGroundD_R, WhittedGroundD_G, WhittedGroundD_B;

   GroundR = 136;
   GroundG = 68;
   GroundB = 0;

   // Color A: Yellow
   WhittedGroundA_R = 255;
   WhittedGroundA_G = 255;
   WhittedGroundA_B = 0;
   // Color B: Red
   WhittedGroundB_R = 255;
   WhittedGroundB_G = 0;
   WhittedGroundB_B = 0;
   // COlor D: Blended in the distance
   WhittedGroundD_R = 255;
   WhittedGroundD_G = 128;
   WhittedGroundD_B = 0;

   ShadeR = 0.1985294117647059;
   ShadeG = ShadeB = 0.2058823529411765;

   /* Calculate aspect ratio value. */

   ar = (scrw * ((scrh * 4.0) / (scrw * 3.0))) / scrh;

   /* Squeeze width of viewport with aspect ratio value. */

   vpx = vopts.vpx * ar;
   vpy = vopts.vpy;

   // sneaky way to trigger gingham checkerboard
   if (vopts.scl < 0)
   {
      Gingham   = 1;
      vopts.scl = -vopts.scl;
   }

   /* Calculate actual portion of the screen to use. */

   actw = scrw * vopts.scl;
   acth = scrh * vopts.scl;
#ifdef WINDOWED_UI
   SDL_SetRenderDrawColor(rp, 0, 0, 0, 0);
#endif // WINDOWED_UI
   for (i = 0; i < actw; i++)
   {
      for (j = 0; j < acth; j++)
      {
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

         for (k = 0; k < npoly; k++)
         {
            if (polygonhit(&ray, &polys[k], &isec))
            {
               l = 1;
            }

            continue;
         }

         if (groundhit(&ray, &isec))
         {
            if (Gingham)
            {
               int xTrue = (abs(fmod(isec.ix + 1000000, 2000.0)) > 1000.0);
               int zTrue = (abs(fmod(isec.iz + 1000000, 2000.0)) > 1000.0);

               if (xTrue ^ zTrue)
               {
                  // Whitted Yellow
                  GroundR = WhittedGroundA_R;
                  GroundG = WhittedGroundA_G;
                  GroundB = WhittedGroundA_B;
               }
               else
               {
                  // Whitted Red
                  GroundR = WhittedGroundB_R;
                  GroundG = WhittedGroundB_G;
                  GroundB = WhittedGroundB_B;
               }
               // do some distance blending of the gingham texture
               if (isec.iz < -4000.0f)
               {
                  float blendFactor = fminf((isec.iz + 4000.0f) / -18000.0f, 1.0f);
                  GroundR = (GroundR * (1.0 - blendFactor)) + (WhittedGroundD_R * blendFactor);
                  GroundG = (GroundG * (1.0 - blendFactor)) + (WhittedGroundD_G * blendFactor);
                  GroundB = (GroundB * (1.0 - blendFactor)) + (WhittedGroundD_B * blendFactor);
               }
            }
            if (shadowchk(&isec))
            {
               color.r = GroundR * ShadeR;
               color.g = GroundG * ShadeG;
               color.b = GroundB * ShadeB;
            }
            else
            {
               color.r = GroundR;
               color.g = GroundG;
               color.b = GroundB;
            }

            storeRGB(&color, i, j);

            l = -1;
         }

         if (l == 1)
         {
            shadepoint(&isec, &color);

            storeRGB(&color, i, j);
         }
         else

         if (l == 0)
         {
            shadesky(&ray, &color);

            storeRGB(&color, i, j);
         }
#ifdef WINDOWED_UI
         SDL_SetRenderDrawColor(rp, (Uint8)color.r, (Uint8)color.g, (Uint8)color.b, 255);
         SDL_RenderDrawPoint(rp, i, j);
         // present graphics and run event loop
         static unsigned int PresentInterval;
         // only do the work every 6400 pixels traced, to minimize overhead
         if (!((++PresentInterval + 1) % 6400))
         {
            SDL_RenderPresent(rp);
            if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
            {
               return;    // kind of an abrupt exit, but the code is simple enough to do it this way
            }
         }
#endif // WINDOWED_UI
         continue;
      }
   }

   return;
}

void interpolatevopts(struct ViewOpts *dest, struct ViewOpts *start, struct ViewOpts *end, float stepfraction)
{
/*
 * not all params make sense to interpolate, so we'll only do what makes sense
 * [camera] cax, cay, caz, [lookposition] lpx, lpy, lpz, [lightsource] lsx, lsy, lsz, [groundpos] wdy, [center of projection] cpd
 */
   // CAmera position
   dest->cax = start->cax + ((end->cax - start->cax) * stepfraction);
   dest->cay = start->cay + ((end->cay - start->cay) * stepfraction);
   dest->caz = start->caz + ((end->caz - start->caz) * stepfraction);

   // Look Position
   dest->lpx = start->lpx + ((end->lpx - start->lpx) * stepfraction);
   dest->lpy = start->lpy + ((end->lpy - start->lpy) * stepfraction);
   dest->lpz = start->lpz + ((end->lpz - start->lpz) * stepfraction);

   // Light Source
   dest->lsx = start->lsx + ((end->lsx - start->lsx) * stepfraction);
   dest->lsy = start->lsy + ((end->lsy - start->lsy) * stepfraction);
   dest->lsz = start->lsz + ((end->lsz - start->lsz) * stepfraction);

   // WorlD
   dest->wdy = start->wdy + ((end->wdy - start->wdy) * stepfraction);

   // Center of Projection
   dest->cpd = start->cpd + ((end->cpd - start->cpd) * stepfraction);

   // Copy over other parameters uninterpolated
   dest->scl = start->scl;
   dest->vpx = start->vpx;
   dest->vpy = start->vpy;
}
