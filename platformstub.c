// stub code to implement Amiga-platform-like APIs temporarily until later refactoring.

#include "platformstub.h"

#include <stdlib.h>


// Memory mamnagement

void *AllocMem(ULONG byteSize, ULONG attributes)
{
    if(attributes & MEMF_CLEAR)
    { // request clearing of allocated memory
        return(calloc(1, byteSize));
    }
    else
    {
        return(malloc(byteSize));
    }

}

void FreeMem(void *memoryBlock, ULONG byteSize)
{
    free(memoryBlock);
}


// File IO
FILE *Open(const char *filename, LONG accessMode )
{
    // only MODE_NEWFILE is supported
    if(accessMode & MODE_NEWFILE)
    {
        return(fopen(filename, "wb"));
    }
return NULL;
}

bool Close( FILE *file )
{
    return(!fclose(file)); // Amiga returns true for success, cstdio returns 0 for success
}

LONG Write(FILE *file , void *buffer, LONG length)
{
    return(fwrite(buffer, length, 1, file));
}
