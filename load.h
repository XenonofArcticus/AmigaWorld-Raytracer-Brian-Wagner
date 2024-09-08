#include "tracer.h"
void convertcol(long col, struct Polygon *poly);

/* Load GEO object and store in the previously allocated buffers.  Vertex */
/* arrays are allocated during the load since the number of vertices per */
/* polygon can vary greatly. */

short loadobject(char *file);

/* Load viewing options and store in the global 'vopts' structure. */

short loadvopts(char *file);
