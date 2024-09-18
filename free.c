/**
 * @file free.c
 * @brief Functions to manage memory deallocation for raytracer structures.
 */

#include <stddef.h>
#include <stdlib.h>

#include "tracer.h"
#include "free.h"

extern struct Polygon *polys;

extern int            npoly;

/* Free all polygon vertex arrays which are allocated during the object */
/* load function. */

void freevtxarrays()
{
   int i;

   for (i = 0; i < npoly; i++)
   {
      if (polys[i].vtx == NULL)
      {
         continue;
      }

      free(polys[i].vtx);
      polys[i].vtx = NULL;

      continue;
   }

   return;
}

