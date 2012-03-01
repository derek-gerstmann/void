#ifndef __DG_RAW_H__
#define __DG_RAW_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

extern FILE*
OpenReadOnlyRawFile(const char *filename, size_t *count);

extern FILE*
OpenWriteOnlyRawFile(const char *filename);

extern void
CloseRawFile(FILE *fh);

extern size_t
WriteRawFloats(FILE* fh, float* buffer, size_t count);

extern size_t
ReadRawFloats(FILE* fh, float* buffer, size_t count);

extern size_t
WriteRawInts(FILE* fh, int* buffer, size_t count);

extern size_t
ReadRawInts(FILE* fh, int* buffer, size_t count);

extern size_t
WriteRawBytes(FILE* fh, char* buffer, size_t count);

extern size_t
ReadRawBytes(FILE* fh, char* buffer, size_t count);

#endif

