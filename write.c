#include <string.h>
#include <stdio.h>

#include "tracer.h"

#ifdef OUTPUT_PNG
#include <spng.h>
#endif // OUTPUT_PNG

#define WRITE_FILENAME_MAXLEN    500
#define PNG_SUFFIX_TLA "png"
#define PNG_SUFFIX_STR "." PNG_SUFFIX_TLA
#define RED_SUFFIX_TLA "red"
#define RED_SUFFIX_STR "." RED_SUFFIX_TLA
#define GRN_SUFFIX_TLA "grn"
#define GRN_SUFFIX_STR "." GRN_SUFFIX_TLA
#define BLU_SUFFIX_TLA "blu"
#define BLU_SUFFIX_STR "." BLU_SUFFIX_TLA

extern unsigned char *red, *grn, *blu;

extern int           scrw, scrh;

/* Write out the RED, GRN, BLU (RGB) buffers. */

void    saveImageToFile(char *name)
{
   #ifdef OUTPUT_PNG
   writePNG(name);
   #else // !OUTPUT_PNG
   writeRGB(name);
   #endif // !OUTPUT_PNG
}

#ifdef OUTPUT_PNG
int writePNG(char *name)
{
   char          file[WRITE_FILENAME_MAXLEN];
   int           err;
   FILE          *fh;
   unsigned char *image;
   size_t        length;
   // inspired by https://github.com/randy408/libspng/blob/v0.7.3/examples/example.c

   int              fmt;
   int              ret  = 0;
   spng_ctx         *ctx = NULL;
   struct spng_ihdr ihdr = { 0 }; /* zero-initialize to set valid defaults */

   /* Creating an encoder context requires a flag */
   ctx = spng_ctx_new(SPNG_CTX_ENCODER);

   /* Encode to internal buffer managed by the library */
   spng_set_option(ctx, SPNG_ENCODE_TO_BUFFER, 1);

   /* Alternatively you can set an output FILE* or stream with spng_set_png_file() or spng_set_png_stream() */

   /* Set image properties, this determines the destination image format */
   ihdr.width      = scrw;
   ihdr.height     = scrh;
   ihdr.color_type = SPNG_COLOR_TYPE_TRUECOLOR;
   ihdr.bit_depth  = 8;
   /* Valid color type, bit depth combinations: https://www.w3.org/TR/2003/REC-PNG-20031110/#table111 */

   spng_set_ihdr(ctx, &ihdr);

   /* When encoding fmt is the source format */
   /* SPNG_FMT_PNG is a special value that matches the format in ihdr */
   fmt = SPNG_FMT_PNG;

   // extra step here -- tracer provides three discrete arrays for R, G, B
   // but PNG code wants one triple-sized interleaved R,G,B,R,G,B,etc
   // so we allocate one triple-sized array, interleave with a loop, and then free when done


   length = scrw * scrh * 3;
   if (image = malloc(length))
   {
      // perform interleaving
      for (unsigned int outbyte = 0, yrow = 0; yrow < scrh; yrow++)
      {
         for (unsigned int xcol = 0; xcol < scrw; xcol++)
         {
            size_t imageoffset = yrow * scrw + xcol;
            image[outbyte++] = red[imageoffset];
            image[outbyte++] = grn[imageoffset];
            image[outbyte++] = blu[imageoffset];
         }
      }

      /* SPNG_ENCODE_FINALIZE will finalize the PNG with the end-of-file marker */
      ret = spng_encode_image(ctx, image, length, fmt, SPNG_ENCODE_FINALIZE);

      if (ret)
      {
         printf("spng_encode_image() error: %s\n", spng_strerror(ret));
         goto encode_error;
      }

      size_t png_size;
      void   *png_buf = NULL;

      /* Get the internal buffer of the finished PNG */
      png_buf = spng_get_png_buffer(ctx, &png_size, &ret);

      if (png_buf == NULL)
      {
         printf("spng_get_png_buffer() error: %s\n", spng_strerror(ret));
      }

      snprintf(file, WRITE_FILENAME_MAXLEN - strlen(PNG_SUFFIX_STR), "%s", name);

      strncat(file, PNG_SUFFIX_STR, WRITE_FILENAME_MAXLEN - (strlen(file) - 1));

      fh = fopen(file, "wb");

      if (fh != NULL)
      {
         fwrite(png_buf, png_size, 1, fh);
         fclose(fh);
      }

      /* User owns the buffer after a successful call */
      free(png_buf);

      free(image);
   }


 encode_error:

   spng_ctx_free(ctx);

   return ret;
}
#endif // OUTPUT_PNG

void    writeRGB(char *name)
{
   char file[500];
   int  size, err;
   FILE *fh;

   size = scrw * scrh;

   snprintf(file, WRITE_FILENAME_MAXLEN - strlen(RED_SUFFIX_STR), "%s", name);

   strncat(file, RED_SUFFIX_STR, WRITE_FILENAME_MAXLEN - (strlen(file) - 1));

   fh = fopen(file, "wb");

   if (fh == NULL)
   {
      puts("Unable to open RED file");

      return;
   }

   err = fwrite(red, size, 1, fh);

   if (err == -1)
   {
      fclose(fh);

      puts("Error writing RED file");

      return;
   }

   fclose(fh);

   snprintf(file, WRITE_FILENAME_MAXLEN - strlen(GRN_SUFFIX_STR), "%s", name);

   strncat(file, GRN_SUFFIX_STR, WRITE_FILENAME_MAXLEN - (strlen(file) - 1));

   fh = fopen(file, "wb");

   if (fh == NULL)
   {
      puts("Unable to open GRN file");

      return;
   }

   err = fwrite(grn, size, 1, fh);

   if (err == -1)
   {
      fclose(fh);

      puts("Error writing GRN file");

      return;
   }

   fclose(fh);

   snprintf(file, WRITE_FILENAME_MAXLEN - strlen(BLU_SUFFIX_STR), "%s", name);

   strncat(file, BLU_SUFFIX_STR, WRITE_FILENAME_MAXLEN - (strlen(file) - 1));

   fh = fopen(file, "wb");

   if (fh == NULL)
   {
      puts("Unable to open BLU file");

      return;
   }

   err = fwrite(blu, size, 1, fh);

   if (err == -1)
   {
      fclose(fh);

      puts("Error writing BLU file");

      return;
   }

   fclose(fh);

   return;
}

