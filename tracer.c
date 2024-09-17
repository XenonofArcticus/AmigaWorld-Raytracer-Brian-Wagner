#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#ifdef WINDOWED_UI
#include <SDL2/SDL.h>
#endif // WINDOWED_UI

#include "tracer.h"
#include "math.h"
#include "free.h"
#include "load.h"
#include "image.h"
#include "write.h"

struct ViewOpts vopts, vopts_start, vopts_end; // vopts is the current vopts

struct Polygon  *polys = NULL;

struct Vertex   *verts      = NULL;
struct Vertex   *cleanverts = NULL;

unsigned char   *red = NULL, *grn = NULL, *blu = NULL;

int             npoly, nvert;

int             widtharg = 0, heightarg = 0, scrw = 0, scrh = 0, numframes = 0;

float           gnx, gny, gnz;

float           timestepfrac = 0.0f, currenttimestep = 0.0f;

#ifdef WINDOWED_UI
int SDL_main(int argc, char *argv[])
{
   SDL_Renderer *rp = NULL;
   SDL_Window   *wp = NULL;
#else // !WINDOWED_UI
int main(argc, argv)
int argc;
char **argv;
{
#endif // !WINDOWED_UI

   int err          = 0;
   int returnStatus = 0; // exit value from main

   /* Initialize that no object exists. */
   npoly = nvert = 0;

#ifdef WINDOWED_UI
   if (SDL_Init(SDL_INIT_VIDEO)) // SDL_Init returns non-zero for failure
   {
      puts("Failed to SDL_Init");
      returnStatus = 1;    // failure
   }
   else  // continue initializing cmdline arg count checking
#endif // WINDOWED_UI
   { // this becomes an else if WINDOWED_UI is defined, otherwise it's an innocuous context push
      /* Check for correct number of arguments. */
      if (argc != 7)
      {
         puts("Usage: tracer objectfile startviewoptsfile endviewoptsfile width height frames");
         returnStatus = 1; // failure
      }
      else                 // continue initializing pixel dimensions
      {
         /* Fetch pixel dimensions and frames. */
         widtharg  = atoi(argv[4]);
         heightarg = atoi(argv[5]);
         numframes = atoi(argv[6]);

         if (widtharg <= 0 || heightarg <= 0 || numframes <= 0)
         {
            puts("Illegal dimensions or frames specified");
            returnStatus = 1; // failure
         }
         else                 // continue initializing polys
         {
            /* Allocate object storage buffers. */
            polys = calloc(MAXPOLYS, sizeof(struct Polygon));

            if (polys == NULL)
            {
               puts("Unable to allocate 'polys' buffer");
               returnStatus = 1; // failure
            }
            else                 // continue initializing verts
            {
               verts      = calloc(MAXVERTS, sizeof(struct Vertex));
               cleanverts = calloc(MAXVERTS, sizeof(struct Vertex));

               if (verts == NULL || cleanverts == NULL)
               {
                  puts("Unable to allocate 'verts' buffer");
                  returnStatus = 1; // failure
               }
               else                 // continue initializing RGB buffers
               {
                  /* Assign screen dimensions */
                  scrw = widtharg;
                  scrh = heightarg;
                  // compute how much from start to end should we step on each frame increment
                  timestepfrac = 1.0f / numframes;

                  /* Allocate RGB buffers. */
                  red = calloc(1, scrw * scrh);
                  grn = calloc(1, scrw * scrh);
                  blu = calloc(1, scrw * scrh);
                  if (red == NULL || grn == NULL || blu == NULL)
                  {
                     puts("Unable to allocate RGB buffers");
                     returnStatus = 1; // failure
                  }
                  else                 // continue initializing Window
                  {
                                       #ifdef WINDOWED_UI
                     /* Open window. */
                     if (SDL_CreateWindowAndRenderer(scrw, scrh, 0, &wp, &rp) == -1)                  // returns -1 on fail and wp=NULL
                     {
                        puts("Unable to open window");
                        returnStatus = 1;                      // failure
                     }
                     else
                                       #endif // WINDOWED_UI
                     {                                             // continue with initialization by loading object
                        /* Load object to be displayed. */
                        if ((err = loadobject(argv[1], cleanverts)) == 1) // err is non-zero for failure
                        {
                           if (err == 1)
                           {
                              puts("Error loading object");
                           }
                           else
                           if (err == 2)
                           {
                              puts("Not a valid GEO object file");
                           }
                           else
                           if (err == 3)
                           {
                              puts("Maximum vertex count exceeded");
                           }
                           else
                           if (err == 4)
                           {
                              puts("Not enough memory for vertex arrays");
                           }
                           else
                           if (err == 5)
                           {
                              puts("Maximum polygon count exceeded");
                           }
                           returnStatus = 1;                         // failure
                        }
                        else
                        {                                           // continue with initializing Loading viewing options
                           /* Load viewing options. */
                           err  = loadvopts(argv[2], &vopts_start); // start vopts
                           err += loadvopts(argv[3], &vopts_end);   // end vopts
                           if (err)                                 // either vopts load failing will trigger this
                           {
                              puts("Unable to load viewing options files");
                              returnStatus = 1;         // failure
                           }
                           else                         // continue with rendering
                           {
                              // initialize the current vopts from the start vopts
                              memcpy(&vopts, &vopts_start, sizeof(struct ViewOpts));

                              /* start timing computation */
                              clock_t start_t, end_t;
                              double  total_t;
                              start_t = clock();

                              /* Initialize the ground's normal to point straight "up". */
                              gnx = 0.0;
                              gny = 1.0;
                              gnz = 0.0;

                              for (int currentframe = 0; currentframe < numframes; currentframe++)
                              {
                                 // refresh clean copy of verts from cleanverts
                                 memcpy(verts, cleanverts, (MAXVERTS * sizeof(struct Vertex)));
                                 interpolatevopts(&vopts, &vopts_start, &vopts_end, currenttimestep);

                                 /* Call each function in the display process. */
                                 transform();
                                 calcnormals();
                                 traceimage(
                                                         #ifdef WINDOWED_UI
                                    rp
                                                         #endif // WINDOWED_UI
                                    );

                                 // save frames during animation
                                 if (numframes > 1)
                                 {
                                    char framefilename[255];                            // poor practice making this fixed size...
                                    sprintf(framefilename, "%s%05d", argv[1], currentframe);
                                    saveImageToFile(framefilename);
                                 }
                                 currenttimestep += timestepfrac;
                              }

                              end_t   = clock();
                              total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
                              printf("Total time taken by CPU: %f\n", total_t);

                              // save at the end for non-animated renders
                              if (numframes == 1)
                              {
                                 saveImageToFile(argv[1]);
                              }
                           }                   // rendering
                        }                      // loading viewing options

                        // it appears it is intended we call freevtxarrays for success or failure of loadobject
                        freevtxarrays();

                        // cleanup Renderer and Window if they were allocated
                                             #ifdef WINDOWED_UI
                        SDL_DestroyRenderer(rp);
                        SDL_DestroyWindow(wp);
                                             #endif // WINDOWED_UI
                     }             // loading object
                  }                // initializing Window

                  // free RGB buffers
                  // this doesn't go inside the next nesting level because any one or more of the three could fail
                  free(red);
                  free(grn);
                  free(blu);

                  // cleanup verts and cleanverts when done or failed further in
                  free(verts);
                  free(cleanverts);
               }              // initializing RGB buffers
               // cleanup polys when done or failed further in
               free(polys);
            } // initializing verts
         }    // initializing object storage buffers
      }       // initializing pixel dimensions
   } // checking correct number of args
#ifdef WINDOWED_UI
   SDL_Quit();
#endif // WINDOWED_UI
   return returnStatus;
}