#ifdef WINDOWED_UI
#include <SDL2/SDL.h>
#endif // !WINDOWED_UI

/* Store color in the RED, GRN, BLU (RGB) buffers. */

void storeRGB(struct Color *c, short x, short y);

void traceimage(
#ifdef WINDOWED_UI
    SDL_Renderer *rp
#endif // WINDOWED_UI
    );
