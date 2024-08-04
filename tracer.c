#include <stdlib.h>

#include "types.h"
#ifdef WINDOWED_UI
#include <SDL2/SDL.h>
#endif // WINDOWED_UI

#include "platformstub.h"

#include "tracer.h"
#include "math.h"
#include "free.h"
#include "load.h"
#include "image.h"
#include "write.h"
#include <time.h>


struct ViewOpts vopts;

struct Polygon *polys;

struct Vertex *verts;

UBYTE *red, *grn, *blu;

SHORT npoly, nvert;

SHORT scrm, scrw, scrh;

FLOAT gnx, gny, gnz;

#ifdef WINDOWED_UI
int SDL_main(int argc, char *argv[])
{
    SDL_Renderer *rp;
    SDL_Window *wp;
#else // !WINDOWED_UI
VOID main(argc, argv)
SHORT argc;
CHAR **argv;
{
#endif // !WINDOWED_UI

   LONG size1, size2, size3,
     err;
#ifdef WINDOWED_UI
      if(SDL_Init(SDL_INIT_VIDEO)) // SDL_Init returns non-zero for failure
      {
      puts("Failed to SDL_Init");
      exit(1); // exit immediately if SDL doesn't initialize
      }
#endif // WINDOWED_UI

   /* Check for correct number of arguments. */

   if (argc != 4) {
      puts("Usage: tracer objectfile viewoptsfile screenmode");

#ifdef WINDOWED_UI
      SDL_Quit();
#endif // WINDOWED_UI
      return;
   }

   /* Make sure screen mode is valid. */

   scrm = atoi(argv[3]);

   if (scrm <= 0 || scrm > 4) {
      puts("Illegal screen mode specified");

#ifdef WINDOWED_UI
      SDL_Quit();
#endif // WINDOWED_UI
      return;
   }


   /* Allocate object storage buffers. */

   size1 = sizeof(struct Polygon) * MAXPOLYS;

   polys = AllocMem(size1, MEMF_CLEAR);

   if (polys == NULL) {

      puts("Unable to allocate 'polys' buffer");

#ifdef WINDOWED_UI
      SDL_Quit();
#endif // WINDOWED_UI
      return;
   }

   size2 = sizeof(struct Vertex) * MAXVERTS;

   verts = AllocMem(size2, MEMF_CLEAR);

   if (verts == NULL) {
      FreeMem(polys, size1);

      puts("Unable to allocate 'verts' buffer");

#ifdef WINDOWED_UI
      SDL_Quit();
#endif // WINDOWED_UI
      return;
   }

   /* Assign screen dimensions based on screen mode. */

   if (scrm == 1) {
      scrw = 320;
      scrh = 200;
   } else

   if (scrm == 2) {
      scrw = 320;
      scrh = 400;
   } else

   if (scrm == 3) {
      scrw = 640;
      scrh = 200;
   } else

   if (scrm == 4) {
      scrw = 640;
      scrh = 400;
   }

   /* Allocate RGB buffers. */

   size3 = (LONG)scrw * scrh;

   red = AllocMem(size3, MEMF_CLEAR);
   grn = AllocMem(size3, MEMF_CLEAR);
   blu = AllocMem(size3, MEMF_CLEAR);

   if (red == NULL || grn == NULL || blu == NULL) {
      if (red) FreeMem(red, size3);
      if (grn) FreeMem(grn, size3);
      if (blu) FreeMem(blu, size3);

      FreeMem(verts, size2);

      FreeMem(polys, size1);

      puts("Unable to allocate RGB buffers");

#ifdef WINDOWED_UI
      SDL_Quit();
#endif // WINDOWED_UI
      return;
   }

#ifdef WINDOWED_UI
   /* Open window. */

   SDL_CreateWindowAndRenderer(scrw, scrh, 0, &wp, &rp); // returns -1 on fail and wp=NULL

   if (wp == NULL) {

      FreeMem(red, size3);
      FreeMem(grn, size3);
      FreeMem(blu, size3);

      FreeMem(verts, size2);

      FreeMem(polys, size1);

      puts("Unable to open window");

#ifdef WINDOWED_UI
      SDL_Quit();
#endif // WINDOWED_UI
      return;
   }
#endif // WINDOWED_UI

   /* Initialize that no object exists. */

   npoly = nvert = 0;

   /* Initialize the ground's normal to point straight "up". */

   gnx = 0.0;
   gny = 1.0;
   gnz = 0.0;

   /* Load object to be displayed. */

   err = loadobject(argv[1]);

   if (err) {
      freevtxarrays();

#ifdef WINDOWED_UI
    SDL_DestroyRenderer(rp);
    SDL_DestroyWindow(wp);
#endif // WINDOWED_UI

      FreeMem(red, size3);
      FreeMem(grn, size3);
      FreeMem(blu, size3);

      FreeMem(verts, size2);

      FreeMem(polys, size1);

      if (err == 1) puts("DOS Error loading object");
      else
      if (err == 2) puts("Not a valid GEO object file");
      else
      if (err == 3) puts("Maximum vertex count exceeded");
      else
      if (err == 4) puts("Not enough memory for vertex arrays");
      else
      if (err == 5) puts("Maximum polygon count exceeded");

#ifdef WINDOWED_UI
      SDL_Quit();
#endif // WINDOWED_UI
      return;
   }

   /* Load viewing options. */

   err = loadvopts(argv[2]);

   if (err) {
      freevtxarrays();

#ifdef WINDOWED_UI
    SDL_DestroyRenderer(rp);
    SDL_DestroyWindow(wp);
#endif // WINDOWED_UI

      FreeMem(red, size3);
      FreeMem(grn, size3);
      FreeMem(blu, size3);

      FreeMem(verts, size2);

      FreeMem(polys, size1);

#ifdef WINDOWED_UI

#endif // WINDOWED_UI

      puts("DOS error loading view file");

#ifdef WINDOWED_UI
      SDL_Quit();
#endif // WINDOWED_UI

      return;
   }

   /* start timing computation */
   clock_t start_t, end_t;
   double total_t;
   start_t = clock();

   /* Call each function in the display process. */

   transform();

   calcnormals();

   traceimage(
#ifdef WINDOWED_UI
      rp
#endif / WINDOWED_UI
      );

   end_t = clock();
   total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
   printf("Total time taken by CPU: %f\n", total_t  );
   
   writeRGB(argv[1]);

   /* Free all allocations & close all openings. */

   freevtxarrays();

#ifdef WINDOWED_UI
    SDL_DestroyRenderer(rp);
    SDL_DestroyWindow(wp);
#endif // WINDOWED_UI

   FreeMem(red, size3);
   FreeMem(grn, size3);
   FreeMem(blu, size3);

   FreeMem(verts, size2);

   FreeMem(polys, size1);

#ifdef WINDOWED_UI
   SDL_Quit();
#endif // WINDOWED_UI

   return;
}

