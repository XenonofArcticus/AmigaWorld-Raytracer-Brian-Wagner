#include <string.h>
#include "types.h"
//#include "intuition/intuition.h"
//#include "libraries/dos.h"
//#include "functions.h"

#include "tracer.h"
#include "platformstub.h"

extern UBYTE *red, *grn, *blu;

extern SHORT scrw, scrh;

/* Write out the RED, GRN, BLU (RGB) buffers. */

VOID writeRGB(name)
CHAR *name;
{
   CHAR file[500];
   LONG size, *fh, err;

   size = (LONG)scrw * scrh;

   strcpy(file, name);

   strcat(file, ".red");

   fh = (LONG *)Open(file, MODE_NEWFILE);

   if (fh == NULL) {
      puts("Unable to open RED file");

      return;
   }

   err = Write(fh, red, size);

   if (err == -1) {
      Close(fh);

      puts("Error writing RED file");

      return;
   }

   Close(fh);

   strcpy(file, name);

   strcat(file, ".grn");

   fh = (LONG *)Open(file, MODE_NEWFILE);

   if (fh == NULL) {
      puts("Unable to open GRN file");

      return;
   }

   err = Write(fh, grn, size);

   if (err == -1) {
      Close(fh);

      puts("Error writing GRN file");

      return;
   }

   Close(fh);

   strcpy(file, name);

   strcat(file, ".blu");

   fh = (LONG *)Open(file, MODE_NEWFILE);

   if (fh == NULL) {
      puts("Unable to open BLU file");

      return;
   }

   err = Write(fh, blu, size);

   if (err == -1) {
      Close(fh);

      puts("Error writing BLU file");

      return;
   }

   Close(fh);

   return;
}

