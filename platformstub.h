#include "types.h"
#include <stdio.h>
#include <stdbool.h>

// memory management

// MEMF_CLEAR is the only supported memory flag
#define MEMF_CLEAR   (1L<<16)	/* AllocMem: NULL out area before return */
void *AllocMem(ULONG byteSize, ULONG attributes);
void FreeMem(void *memoryBlock, ULONG byteSize);


// file IO

// accessMode flags
/* Mode parameter to Open() */
#define MODE_OLDFILE	     1005   /* Open existing file read/write
				     * positioned at beginning of file. */
#define MODE_NEWFILE	     1006   /* Open freshly created file (delete
				     * old file) read/write, exclusive lock. */
#define MODE_READWRITE	     1004   /* Open old file w/shared lock,
				     * creates file if doesn't exist. */

// Amiga Open takes STRPTR and returns BPTR, we'll use compatible stand-ins
// only MODE_NEWFILE is supported
FILE *Open(const char *filename, LONG accessMode );
// Amiga Close takes BPTR, but we're substituting for cstdio FILE *
bool Close( FILE *file );
// returnedLength =  Write( file, buffer, length )
LONG Write (FILE *file , void *buffer, LONG length);
