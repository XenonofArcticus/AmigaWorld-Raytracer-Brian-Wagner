#include <stdlib.h>

#ifdef WINDOWED_UI
#include <SDL2/SDL.h>
#endif // WINDOWED_UI

#include "tracer.h"
#include "math.h"
#include "free.h"
#include "load.h"
#include "image.h"
#include "write.h"
#include <time.h>

struct ViewOpts vopts;

struct Polygon *polys = NULL;

struct Vertex *verts = NULL;

unsigned char *red = NULL, *grn = NULL, *blu = NULL;

int npoly, nvert;

int widtharg, heightarg, scrw, scrh;

float gnx, gny, gnz;

#ifdef WINDOWED_UI
int SDL_main(int argc, char *argv[])
{
    SDL_Renderer *rp = NULL;
    SDL_Window *wp = NULL;
#else // !WINDOWED_UI
void main(argc, argv)
int argc;
char **argv;
{
#endif // !WINDOWED_UI

   int err = 0;
   int returnStatus = 0; // exit valud from main

   /* Initialize that no object exists. */
   npoly = nvert = 0;

#ifdef WINDOWED_UI
      if(SDL_Init(SDL_INIT_VIDEO)) // SDL_Init returns non-zero for failure
      {
            puts("Failed to SDL_Init");
            returnStatus = 1; // failure 
      }
      else  // continue initializing cmdline arg count checking
#endif // WINDOWED_UI
      { // this becomes an else if WINDOWED_UI is defined, otherwise it's an innocuous context push
            /* Check for correct number of arguments. */
            if (argc != 5)
            {
                  puts("Usage: tracer objectfile viewoptsfile width height");
                  returnStatus = 1; // failure 
            }
            else // continue initializing pixel dimensions
            {
                  /* Fetch pixel dimensions. */
                  widtharg = atoi(argv[3]);
                  heightarg = atoi(argv[4]);

                  if (widtharg <= 0 || heightarg <= 0)
                  {
                        puts("Illegal dimensions specified");
                        returnStatus = 1; // failure 
                  }
                  else // continue initializing polys
                  {
                        /* Allocate object storage buffers. */
                        polys = calloc(MAXPOLYS, sizeof(struct Polygon));

                        if (polys == NULL)
                        {
                              puts("Unable to allocate 'polys' buffer");
                              returnStatus = 1; // failure 
                        }
                        else // continue initializing verts
                        {
                              verts = calloc(MAXVERTS, sizeof(struct Vertex));

                              if (verts == NULL)
                              {
                                    puts("Unable to allocate 'verts' buffer");
                                    returnStatus = 1; // failure
                              }
                              else // continue initializing RGB buffers
                              {
                                    /* Assign screen dimensions */
                                    scrw = widtharg;
                                    scrh = heightarg;

                                    /* Allocate RGB buffers. */
                                    red = calloc(1, scrw * scrh);
                                    grn = calloc(1, scrw * scrh);
                                    blu = calloc(1, scrw * scrh);
                                    if (red == NULL || grn == NULL || blu == NULL)
                                    {
                                          puts("Unable to allocate RGB buffers");
                                          returnStatus = 1; // failure 
                                    }
                                    else // continue initializing Window
                                    {
                                          #ifdef WINDOWED_UI
                                          /* Open window. */
                                          if(SDL_CreateWindowAndRenderer(scrw, scrh, 0, &wp, &rp) == -1) // returns -1 on fail and wp=NULL
                                          {
                                                puts("Unable to open window");
                                                returnStatus = 1; // failure 
                                          }
                                          else
                                          #endif // WINDOWED_UI
                                          { // continue with initialization by loading object
                                                /* Load object to be displayed. */
                                                if(err = loadobject(argv[1])) // err is non-zero for failure
                                                {
                                                      if (err == 1) puts("Error loading object");
                                                      else
                                                      if (err == 2) puts("Not a valid GEO object file");
                                                      else
                                                      if (err == 3) puts("Maximum vertex count exceeded");
                                                      else
                                                      if (err == 4) puts("Not enough memory for vertex arrays");
                                                      else
                                                      if (err == 5) puts("Maximum polygon count exceeded");
                                                      returnStatus = 1; // failure 
                                                }
                                                else
                                                { // continue with initializing Loading viewing options
                                                      /* Load viewing options. */
                                                      if(err = loadvopts(argv[2]))
                                                      {
                                                            puts("Unable to load viewing options file");
                                                            returnStatus = 1; // failure 
                                                      }
                                                      else // continue with rendering
                                                      {
                                                            /* start timing computation */
                                                            clock_t start_t, end_t;
                                                            double total_t;
                                                            start_t = clock();

                                                            /* Initialize the ground's normal to point straight "up". */
                                                            gnx = 0.0;
                                                            gny = 1.0;
                                                            gnz = 0.0;

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
                                                            
                                                            saveImageToFile(argv[1]);
                                                      } // rendering
                                                } // loading viewing options

                                                // it appears it is intended we call freevtxarrays for success or failure of loadobject
                                                freevtxarrays();
                                                
                                                // cleanup Renderer and Window if they were allocated
                                                #ifdef WINDOWED_UI
                                                SDL_DestroyRenderer(rp);
                                                SDL_DestroyWindow(wp);
                                                #endif // WINDOWED_UI
                                          } // loading object
                                    } // initializing Window

                                    // free RGB buffers
                                    // this doesn't go inside the next nesting level because any one or more of the three could fail
                                    free(red);
                                    free(grn);
                                    free(blu);

                                    // cleanup verts when done or failed further in
                                    free(verts);
                              }  // initializing RGB buffers
                              // cleanup polys when done or failed further in
                              free(polys);
                        } // initializing verts
                  } // initializing object storage buffers
            } // initializing pixel dimensions
      } // checking correct number of args
#ifdef WINDOWED_UI
   SDL_Quit();
#endif // WINDOWED_UI
   return returnStatus;
}