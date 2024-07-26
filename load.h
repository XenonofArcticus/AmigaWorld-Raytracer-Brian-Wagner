#include "types.h"
VOID convertcol(LONG col, struct Polygon *poly);

/* Load GEO object and store in the previously allocated buffers.  Vertex */
/* arrays are allocated during the load since the number of vertices per */
/* polygon can vary greatly. */

SHORT loadobject(CHAR *file);

/* Load viewing options and store in the global 'vopts' structure. */

SHORT loadvopts(CHAR *file);
