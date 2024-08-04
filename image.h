#include "types.h"
#ifdef WINDOWED_UI
#include <SDL2/SDL.h>
#endif // !WINDOWED_UI

/* Store color in the RED, GRN, BLU (RGB) buffers. */

VOID storeRGB(struct Color *c, SHORT x, SHORT y);

VOID traceimage(
#ifdef WINDOWED_UI
    SDL_Renderer *rp
#endif // WINDOWED_UI
    );
