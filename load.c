/**
 * @file load.c
 * @brief Functions to load scene data for the raytracer.
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "load.h"
#include "tracer.h"

extern struct Polygon  *polys;

extern struct ViewOpts vopts;

extern int             npoly, nvert;


// Lookup arrays for R, G, and B values for convertcol
float materialLookupR[16] = { 0, 0, 0, 0, 127, 127, 255, 170, 0, 0, 0, 0, 255, 255, 255, 255 };
float materialLookupG[16] = { 0, 0, 127, 127, 0, 0, 100, 170, 0, 0, 255, 255, 0, 0, 255, 255 };
float materialLookupB[16] = { 0, 127, 0, 127, 0, 127, 0, 170, 0, 255, 0, 255, 0, 255, 0, 255 };

/* Convert GEO color code to equivalent RGB color values. */
void convertcol(int col, struct Polygon *poly)
{
   // Initialize poly with 0,0,0 RGB values
   poly->r = 0;
   poly->g = 0;
   poly->b = 0;

   // Check if col is in the valid range
   if (col >= 1 && col <= 15)
   {
      poly->r = materialLookupR[col];
      poly->g = materialLookupG[col];
      poly->b = materialLookupB[col];
   }

   return;
}

/* Load GEO object and store in the previously allocated buffers.  Vertex */
/* arrays are allocated during the load since the number of vertices per */
/* polygon can vary greatly. */

int     loadobject(char *file, struct Vertex *loadverts)
{
   FILE *fp;

   char id[5];
   int  obid, i, cnt, col;

   fp = fopen(file, "r");

   if (fp == NULL)
   {
      return 1;
   }

   // NOLINTNEXTLINE(clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling)
   fscanf(fp, "%4s\n", id);

   if (ferror(fp))
   {
      fclose(fp);

      return 1;
   }

   obid = 0;

   if (strncmp(id, "3DG1", 4) == 0)
   {
      obid = 1;
   }

   if (obid == 0)
   {
      fclose(fp);

      return 2;
   }

   // NOLINTNEXTLINE(clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling)
   fscanf(fp, "%d\n", &nvert);

   if (ferror(fp))
   {
      fclose(fp);

      return 1;
   }

   for (i = 0; i < nvert; i++)
   {
      // NOLINTNEXTLINE(clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling)
      fscanf(fp, "%f %f %f\n", &loadverts[i].x, &loadverts[i].y, &loadverts[i].z);

      if (ferror(fp))
      {
         fclose(fp);

         return 1;
      }

      loadverts[i].z *= -1;

      if (i < MAXVERTS - 1)
      {
         continue;
      }

      fclose(fp);

      return 3;
   }

   while (1)
   {
      // NOLINTNEXTLINE(clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling)
      fscanf(fp, "%d ", &cnt);

      if (feof(fp))
      {
         break;
      }

      if (ferror(fp))
      {
         fclose(fp);

         return 1;
      }

      polys[npoly].cnt = cnt;

      polys[npoly].vtx = calloc(1, cnt * sizeof(int));

      if (polys[npoly].vtx == NULL)
      {
         fclose(fp);

         return 4;
      }

      for (i = 0; i < cnt; i++)
      {
         // NOLINTNEXTLINE(clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling)
         fscanf(fp, "%d ", &polys[npoly].vtx[i]);

         if (ferror(fp))
         {
            fclose(fp);

            return 1;
         }
      }

      // NOLINTNEXTLINE(clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling)
      fscanf(fp, "%d\n", &col);

      if (ferror(fp))
      {
         fclose(fp);

         return 1;
      }

      if (col < 0)
      {
         col = -col;
      }

      if (col > 15)
      {
         if (col < 32)
         {
            col -= 16;
         }
         else
         if (col < 48)
         {
            col -= 32;
         }
         else
         if (col < 64)
         {
            col -= 48;
         }
      }

      convertcol(col, &polys[npoly]);

      npoly++;

      if (npoly == MAXPOLYS)
      {
         fclose(fp);

         return 5;
      }

      continue;
   }

   fclose(fp);

   return 0;
}


int     loadvopts(char *file, struct ViewOpts *destvopts)
{
   FILE *fp;

   fp = fopen(file, "r");

   if (fp == NULL)
   {
      return 1;
   }

   // NOLINTNEXTLINE(clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling)
   fscanf(fp, "%f %f %f\n", &destvopts->cax, &destvopts->cay, &destvopts->caz);

   if (ferror(fp))
   {
      fclose(fp);

      return 1;
   }

   // NOLINTNEXTLINE(clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling)
   fscanf(fp, "%f %f %f\n", &destvopts->lpx, &destvopts->lpy, &destvopts->lpz);

   if (ferror(fp))
   {
      fclose(fp);

      return 1;
   }

   // NOLINTNEXTLINE(clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling)
   fscanf(fp, "%f\n", &destvopts->scl);

   if (ferror(fp))
   {
      fclose(fp);

      return 1;
   }

   // NOLINTNEXTLINE(clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling)
   fscanf(fp, "%f %f %f\n", &destvopts->lsx, &destvopts->lsy, &destvopts->lsz);

   if (ferror(fp))
   {
      fclose(fp);

      return 1;
   }

   // NOLINTNEXTLINE(clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling)
   fscanf(fp, "%f %f\n", &destvopts->vpx, &destvopts->vpy);

   if (ferror(fp))
   {
      fclose(fp);

      return 1;
   }

   // NOLINTNEXTLINE(clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling)
   fscanf(fp, "%f\n", &destvopts->wdy);

   if (ferror(fp))
   {
      fclose(fp);

      return 1;
   }

   // NOLINTNEXTLINE(clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling)
   fscanf(fp, "%f\n", &destvopts->cpd);

   if (ferror(fp))
   {
      fclose(fp);

      return 1;
   }

   fclose(fp);

   return 0;
}

