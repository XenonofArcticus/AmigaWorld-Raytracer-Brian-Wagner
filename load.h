#include "tracer.h"
void convertcol(int col, struct Polygon *poly);

/* Load GEO object and store in the previously allocated buffers.  Vertex */
/* arrays are allocated during the load since the number of vertices per */
/* polygon can vary greatly. */

int loadobject(char *file, struct Vertex *loadverts);

/* Load viewing options and store in the global 'vopts' structure. */

int loadvopts(char *file, struct ViewOpts *destvopts);
