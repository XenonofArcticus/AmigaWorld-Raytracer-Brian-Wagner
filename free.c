#include <stddef.h>

#include "tracer.h"
#include "free.h"

extern struct Polygon *polys;

extern int npoly;

/* Free all polygon vertex arrays which are allocated during the object */
/* load function. */

void freevtxarrays()
{
   long i;

   for (i = 0; i < npoly; i++) {
      if (polys[i].vtx == NULL) continue;

      free(polys[i].vtx);

      continue;
   }

   return;
}

