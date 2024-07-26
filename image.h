#include "types.h"

/* Store color in the RED, GRN, BLU (RGB) buffers. */

VOID storeRGB(struct Color *c, SHORT x, SHORT y);

VOID traceimage(
#ifdef WINDOWED_UI
    struct RastPort *rp
#endif // WINDOWED_UI
    );
