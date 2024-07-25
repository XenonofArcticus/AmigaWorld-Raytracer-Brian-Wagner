//#include "exec/types.h"
//#include "exec/memory.h"
//#include "intuition/intuition.h"
//#include "functions.h"

#include "tracer.h"

struct GfxBase *GfxBase;

struct IntuitionBase *IntuitionBase;

struct Polygon *polys;

struct Vertex *verts;

struct ViewOpts vopts;

UBYTE *red, *grn, *blu;

SHORT npoly, nvert;

SHORT scrm, scrw, scrh;

FLOAT gnx, gny, gnz;

VOID main(argc, argv)
SHORT argc;
CHAR **argv;
{
   struct NewScreen ns;

   struct NewWindow nw;

   struct Screen *sp;

   struct Window *wp;

   struct RastPort *rp;

   struct ViewPort *vp;

   LONG size1, size2, size3, vmod, err;

   /* Check for correct number of arguments. */

   if (argc != 4) {
      puts("Usage: tracer objectfile viewoptsfile screenmode");

      return;
   }

   /* Make sure screen mode is valid. */

   scrm = atoi(argv[3]);

   if (scrm <= 0 || scrm > 4) {
      puts("Illegal screen mode specified");

      return;
   }

   /* Open graphics & intuition libraries. */

   GfxBase = (VOID *)OpenLibrary("graphics.library", NULL);

   if (GfxBase == NULL) {
      puts("Unable to open graphics.library");

      return;
   }

   IntuitionBase = (VOID *)OpenLibrary("intuition.library", NULL);

   if (IntuitionBase == NULL) {
      CloseLibrary(GfxBase);

      puts("Unable to open intuition.library");

      return;
   }

   /* Allocate object storage buffers. */

   size1 = sizeof(struct Polygon) * MAXPOLYS;

   polys = AllocMem(size1, MEMF_CLEAR);

   if (polys == NULL) {
      CloseLibrary(IntuitionBase);

      CloseLibrary(GfxBase);

      puts("Unable to allocate 'polys' buffer");

      return;
   }

   size2 = sizeof(struct Vertex) * MAXVERTS;

   verts = AllocMem(size2, MEMF_CLEAR);

   if (verts == NULL) {
      FreeMem(polys, size1);

      CloseLibrary(IntuitionBase);

      CloseLibrary(GfxBase);

      puts("Unable to allocate 'verts' buffer");

      return;
   }

   /* Assign screen dimensions based on screen mode. */

   if (scrm == 1) {
      scrw = 320;
      scrh = 200;

      vmod = NULL;
   } else

   if (scrm == 2) {
      scrw = 320;
      scrh = 400;

      vmod = LACE;
   } else

   if (scrm == 3) {
      scrw = 640;
      scrh = 200;

      vmod = HIRES;
   } else

   if (scrm == 4) {
      scrw = 640;
      scrh = 400;

      vmod = HIRES | LACE;
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

      CloseLibrary(IntuitionBase);

      CloseLibrary(GfxBase);

      puts("Unable to allocate RGB buffers");

      return;
   }

   /* Open screen. */

   ns.LeftEdge = 0;
   ns.TopEdge = 0;
   ns.Width = scrw;
   ns.Height = scrh;
   ns.Depth = 1;
   ns.DetailPen = 0;
   ns.BlockPen = 0;
   ns.ViewModes = vmod;
   ns.Type = CUSTOMSCREEN | SCREENQUIET;
   ns.Font = NULL;
   ns.DefaultTitle = NULL;
   ns.Gadgets = NULL;
   ns.CustomBitMap = NULL;

   sp = OpenScreen(&ns);

   if (sp == NULL) {
      FreeMem(red, size3);
      FreeMem(grn, size3);
      FreeMem(blu, size3);

      FreeMem(verts, size2);

      FreeMem(polys, size1);

      CloseLibrary(IntuitionBase);

      CloseLibrary(GfxBase);

      puts("Unable to open screen");

      return;
   }

   /* Open window. */

   nw.LeftEdge = 0;
   nw.TopEdge = 0;
   nw.Width = scrw;
   nw.Height = scrh;
   nw.DetailPen = 0;
   nw.BlockPen = 0;
   nw.IDCMPFlags = RAWKEY;
   nw.Flags = SMART_REFRESH | BACKDROP | BORDERLESS | ACTIVATE | RMBTRAP;
   nw.FirstGadget = NULL;
   nw.CheckMark = NULL;
   nw.Title = NULL;
   nw.Screen = sp;
   nw.BitMap = NULL;
   nw.MinWidth = 0;
   nw.MinHeight = 0;
   nw.MaxWidth = 0;
   nw.MaxHeight = 0;
   nw.Type = CUSTOMSCREEN;

   wp = OpenWindow(&nw);

   if (wp == NULL) {
      CloseScreen(sp);

      FreeMem(red, size3);
      FreeMem(grn, size3);
      FreeMem(blu, size3);

      FreeMem(verts, size2);

      FreeMem(polys, size1);

      CloseLibrary(IntuitionBase);

      CloseLibrary(GfxBase);

      puts("Unable to open window");

      return;
   }

   rp = wp->RPort;

   vp = &sp->ViewPort;

   /* Set screen colors. */

   SetRGB4(vp, (LONG)0, (LONG)0, (LONG)0, (LONG)0);
   SetRGB4(vp, (LONG)1, (LONG)9, (LONG)9, (LONG)9);

   ShowTitle(sp, FALSE);

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

      CloseWindow(wp);

      CloseScreen(sp);

      FreeMem(red, size3);
      FreeMem(grn, size3);
      FreeMem(blu, size3);

      FreeMem(verts, size2);

      FreeMem(polys, size1);

      CloseLibrary(IntuitionBase);

      CloseLibrary(GfxBase);

      if (err == 1) puts("DOS Error loading object");
      else
      if (err == 2) puts("Not a valid GEO object file");
      else
      if (err == 3) puts("Maximum vertex count exceeded");
      else
      if (err == 4) puts("Not enough memory for vertex arrays");
      else
      if (err == 5) puts("Maximum polygon count exceeded");

      return;
   }

   /* Load viewing options. */

   err = loadvopts(argv[2]);

   if (err) {
      freevtxarrays();

      CloseWindow(wp);

      CloseScreen(sp);

      FreeMem(red, size3);
      FreeMem(grn, size3);
      FreeMem(blu, size3);

      FreeMem(verts, size2);

      FreeMem(polys, size1);

      CloseLibrary(IntuitionBase);

      CloseLibrary(GfxBase);

      puts("DOS error loading view file");

      return;
   }

   /* Call each function in the display process. */

   transform();

   calcnormals();

   traceimage(rp);

   writeRGB(argv[1]);

   /* Free all allocations & close all openings. */

   freevtxarrays();

   CloseWindow(wp);

   CloseScreen(sp);

   FreeMem(red, size3);
   FreeMem(grn, size3);
   FreeMem(blu, size3);

   FreeMem(verts, size2);

   FreeMem(polys, size1);

   CloseLibrary(IntuitionBase);

   CloseLibrary(GfxBase);

   return;
}

