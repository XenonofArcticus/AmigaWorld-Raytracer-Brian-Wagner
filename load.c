#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "load.h"
#include "tracer.h"

extern struct Polygon *polys;

extern struct ViewOpts vopts;

extern int npoly, nvert;

/* Convert GEO color code to equivalent RGB color values. */

void 	convertcol (int col, struct Polygon *poly)
{
   switch(col) {
      case 0 : poly->r = 0;
               poly->g = 0;
               poly->b = 0;
               break;
      case 1 : poly->r = 0;
               poly->g = 0;
               poly->b = 127;
               break;
      case 2 : poly->r = 0;
               poly->g = 127;
               poly->b = 0;
               break;
      case 3 : poly->r = 0;
               poly->g = 127;
               poly->b = 127;
               break;
      case 4 : poly->r = 127;
               poly->g = 0;
               poly->b = 0;
               break;
      case 5 : poly->r = 127;
               poly->g = 0;
               poly->b = 127;
               break;
      case 6 : poly->r = 255;
               poly->g = 100;
               poly->b = 0;
               break;
      case 7 : poly->r = 170;
               poly->g = 170;
               poly->b = 170;
               break;
      case 8 : poly->r = 0;
               poly->g = 0;
               poly->b = 0;
               break;
      case 9 : poly->r = 0;
               poly->g = 0;
               poly->b = 255;
               break;
      case 10: poly->r = 0;
               poly->g = 255;
               poly->b = 0;
               break;
      case 11: poly->r = 0;
               poly->g = 255;
               poly->b = 255;
               break;
      case 12: poly->r = 255;
               poly->g = 0;
               poly->b = 0;
               break;
      case 13: poly->r = 255;
               poly->g = 0;
               poly->b = 255;
               break;
      case 14: poly->r = 255;
               poly->g = 255;
               poly->b = 0;
               break;
      case 15: poly->r = 255;
               poly->g = 255;
               poly->b = 255;
               break;
      default: poly->r = 0;
               poly->g = 0;
               poly->b = 0;
               break;
   }

   return;
}

/* Load GEO object and store in the previously allocated buffers.  Vertex */
/* arrays are allocated during the load since the number of vertices per */
/* polygon can vary greatly. */

int 	loadobject (char *file, struct Vertex *loadverts)
{
   FILE *fp;

   char id[5];
   int obid, i, cnt, col;

   fp = fopen(file, "r");

   if (fp == NULL) return(1);

   fscanf(fp, "%4s\n", id);

   if (ferror(fp)) {
      fclose(fp);

      return(1);
   }

   obid = 0;

   if (strncmp(id, "3DG1", 4) == 0) obid = 1;

   if (obid == 0) {
      fclose(fp);

      return(2);
   }

   fscanf(fp, "%d\n", &nvert);

   if (ferror(fp)) {
      fclose(fp);

      return(1);
   }

   for (i = 0; i < nvert; i++) {
      fscanf(fp, "%f %f %f\n", &loadverts[i].x, &loadverts[i].y, &loadverts[i].z);

      if (ferror(fp)) {
         fclose(fp);

         return(1);
      }

      loadverts[i].z *= -1;

      if (i < MAXVERTS - 1) continue;

      fclose(fp);

      return(3);
   }

   while(1) {
      fscanf(fp, "%ld ", &cnt);

      if (feof(fp)) break;

      if (ferror(fp)) {
         fclose(fp);

         return(1);
      }

      polys[npoly].cnt = cnt;

      polys[npoly].vtx = calloc(1, cnt * sizeof(int));

      if (polys[npoly].vtx == NULL) {
         fclose(fp);

         return(4);
      }

      for (i = 0; i < cnt; i++) {
         fscanf(fp, "%d ", &polys[npoly].vtx[i]);

         if (ferror(fp)) {
            fclose(fp);

            return(1);
         }
      }

      fscanf(fp, "%ld\n", &col);

      if (ferror(fp)) {
         fclose(fp);

         return(1);
      }

      if (col < 0) col = -col;

      if (col > 15) {
         if (col < 32) col -= 16;
         else
         if (col < 48) col -= 32;
         else
         if (col < 64) col -= 48;
      }

      convertcol(col, &polys[npoly]);

      npoly++;

      if (npoly == MAXPOLYS) {
         fclose(fp);

         return(5);
      }

      continue;
   }

   fclose(fp);

   return(0);
}

/* Load viewing options and store in the global 'vopts' structure. */

int 	loadvopts (char *file, struct ViewOpts *destvopts)
{
   FILE *fp;

   fp = fopen(file, "r");

   if (fp == NULL) return(1);

   fscanf(fp, "%f %f %f\n", &destvopts->cax, &destvopts->cay, &destvopts->caz);

   if (ferror(fp)) {
      fclose(fp);

      return(1);
   }

   fscanf(fp, "%f %f %f\n", &destvopts->lpx, &destvopts->lpy, &destvopts->lpz);

   if (ferror(fp)) {
      fclose(fp);

      return(1);
   }

   fscanf(fp, "%f\n", &destvopts->scl);

   if (ferror(fp)) {
      fclose(fp);

      return(1);
   }

   fscanf(fp, "%f %f %f\n", &destvopts->lsx, &destvopts->lsy, &destvopts->lsz);

   if (ferror(fp)) {
      fclose(fp);

      return(1);
   }

   fscanf(fp, "%f %f\n", &destvopts->vpx, &destvopts->vpy);

   if (ferror(fp)) {
      fclose(fp);

      return(1);
   }

   fscanf(fp, "%f\n", &destvopts->wdy);

   if (ferror(fp)) {
      fclose(fp);

      return(1);
   }

   fscanf(fp, "%f\n", &destvopts->cpd);

   if (ferror(fp)) {
      fclose(fp);

      return(1);
   }

   fclose(fp);

   return(0);
}

