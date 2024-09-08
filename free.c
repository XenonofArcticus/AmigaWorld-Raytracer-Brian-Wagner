#include "types.h"

#include "tracer.h"
#include "free.h"

extern struct Polygon *polys;

extern SHORT npoly;

/* Free all polygon vertex arrays which are allocated during the object */
/* load function. */

VOID freevtxarrays()
{
   LONG i, size;

   for (i = 0; i < npoly; i++) {
      size = polys[i].cnt * 2;

      if (polys[i].vtx == NULL) continue;

      free(polys[i].vtx);

      continue;
   }

   return;
}

