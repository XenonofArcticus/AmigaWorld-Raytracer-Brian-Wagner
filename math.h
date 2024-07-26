#include <math.h>
#include "types.h"

extern struct Polygon *polys;

extern struct Vertex *verts;

extern struct ViewOpts vopts;

extern SHORT npoly, nvert;

extern SHORT scrw, scrh;

extern FLOAT gnx, gny, gnz;

//extern FLOAT sqrt(); // defined in system math.h

/* Calculate a unit vector by dividing each component by the their */
/* combined length. */

VOID unitvector(FLOAT *x, FLOAT *y, FLOAT *z);

/* Transform object vertices and light source position to the view plane */
/* coordinate system.  This is done to greatly simplify calculations. */

VOID transform();

/* Calculate surface normals for the polygons by using the vector cross */
/* product operation. */

VOID calcnormals();

/* Check to see if a ray and triangle intersect. */

SHORT trianglehit(struct Ray *r, struct Triangle *t, struct Intersection *i);

/* Check to see if a ray and polygon intersect. */

SHORT polygonhit(struct Ray *r, struct Polygon *p, struct Intersection *i);

/* Check to see if a ray hits the ground. */

SHORT groundhit(struct Ray *r, struct Intersection *i);

/* Check to see if ANY polygons lie between a surface point and */
/* the light source. */

SHORT shadowchk(struct Intersection *i);

/* Calculate shade of color for a given surface point. */

VOID shadepoint(struct Intersection *i, struct Color *c);

/* Calculate a sky color based on ray direction. */

VOID shadesky(struct Ray *r, struct Color *c);

