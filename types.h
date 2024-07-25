// types imitating the Amiga's exec/types.h

#ifndef VOID
#define VOID            void
#endif

typedef unsigned char   UBYTE;      /* unsigned 8-bit quantity */
typedef char            CHAR;      /* signed 8-bit quantity */
typedef short           SHORT;      /* signed 16-bit quantity (use WORD) */
typedef unsigned short  USHORT;     /* unsigned 16-bit quantity (use UWORD) */
typedef long            LONG;       /* signed 32-bit quantity */
typedef unsigned long   ULONG;      /* unsigned 32-bit quantity */
typedef float           FLOAT;
typedef double          DOUBLE;
typedef unsigned char   UBYTE;      /* unsigned 8-bit quantity */
#ifndef TRUE
#define TRUE            1
#endif
#ifndef FALSE
#define FALSE           0
#endif
#ifndef NULL
#define NULL            0L
#endif

