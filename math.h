#include <math.h>

extern struct Polygon *polys;

extern struct Vertex *verts;

extern struct ViewOpts vopts;

extern short npoly, nvert;

extern short scrw, scrh;

extern float gnx, gny, gnz;

//extern float sqrt(); // defined in system math.h

/* Calculate a unit vector by dividing each component by the their */
/* combined length. */

void unitvector(float *x, float *y, float *z);

/* Transform object vertices and light source position to the view plane */
/* coordinate system.  This is done to greatly simplify calculations. */

void transform();

/* Calculate surface normals for the polygons by using the vector cross */
/* product operation. */

void calcnormals();

/* Check to see if a ray and triangle intersect. */

short trianglehit(struct Ray *r, struct Triangle *t, struct Intersection *i);

/* Check to see if a ray and polygon intersect. */

short polygonhit(struct Ray *r, struct Polygon *p, struct Intersection *i);

/* Check to see if a ray hits the ground. */

short groundhit(struct Ray *r, struct Intersection *i);

/* Check to see if ANY polygons lie between a surface point and */
/* the light source. */

short shadowchk(struct Intersection *i);

/* Calculate shade of color for a given surface point. */

void shadepoint(struct Intersection *i, struct Color *c);

/* Calculate a sky color based on ray direction. */

void shadesky(struct Ray *r, struct Color *c);

