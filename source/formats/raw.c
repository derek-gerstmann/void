/* ===============================================================================================
//
// License:		The Lesser GNU Public License (LGPL) v3.0.
// 
// Author(s): 	Derek Gerstmann. The University of Western Australia (UWA). 
//				As well as the shoulders of many giants...
//
// This file is part of the Void framework.
//
// The Void framework is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// The Void framework is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with the Void framework.  If not, see <http://www.gnu.org/licenses/>.
//
// ============================================================================================== */

#include "raw.h"

/* ============================================================================================== */

#if defined(VD_TARGET_OSX)
#include <libc.h>
#endif

#include <stdbool.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

#define vdLogGlobalInfo(...) \
    fprintf(stdout, __VA_ARGS__ );

#define vdLogGlobalWarning(...) \
    fprintf(stdout, __VA_ARGS__ );

#define vdLogGlobalError(...) \
    fprintf(stderr, __VA_ARGS__ );

/* ============================================================================================== */

FILE*
OpenReadOnlyRawFile(const char* filename, size_t* count)
{
    struct stat statbuf;
    FILE*        fh;

    fh = fopen(filename, "rb");

    if(fh == 0)
        return 0;

    stat(filename, &statbuf);
    size_t elements = statbuf.st_size;
    *count = elements;
    return fh;
}

/* ============================================================================================== */

FILE*
OpenWriteOnlyRawFile(const char* filename)
{
    FILE*        fh;
    fh = fopen(filename, "wb+");

    if(fh == 0)
        return 0;

    return fh;
}

/* ============================================================================================== */

void
CloseRawFile(FILE* fh)
{
    fclose(fh);
}

/* ============================================================================================== */

size_t
WriteRawFloats(FILE* fh, float* buffer, size_t count)
{
    if(fh == NULL || buffer == NULL || count < 1)
        return 0;

    size_t written = fwrite(buffer, sizeof(float), count, fh);

    if(written != count)
    {
        vdLogGlobalWarning("Wrote only '%u' out of '%u' floats!\n", 
        					(unsigned)written, (unsigned)count);
    }

    return written;
}

/* ============================================================================================== */

size_t
ReadRawFloats(FILE* fh, float* buffer, size_t count)
{
    if(fh == NULL || buffer == NULL || count < 1)
        return 0;

    size_t read = fread(buffer, sizeof(float), count, fh);

    if(read != count)
    {
        vdLogGlobalWarning("Read only '%u' out of '%u' floats!\n", 
        					(unsigned)read, (unsigned)count);
    }

    return read;
}

/* ============================================================================================== */

size_t
WriteRawInts(FILE* fh, int* buffer, size_t count)
{
    if(fh == NULL || buffer == NULL || count < 1)
        return 0;

    size_t written = fwrite(buffer, sizeof(int), count, fh);

    if(written != count)
    {
        vdLogGlobalWarning("Wrote only '%u' out of '%u' ints!\n", 
        					(unsigned)written, (unsigned)count);
    }

    return written;
}

/* ============================================================================================== */

size_t
ReadRawInts(FILE* fh, int* buffer, size_t count)
{
    if(fh == NULL || buffer == NULL || count < 1)
        return 0;

    size_t read = fread(buffer, sizeof(int), count, fh);

    if(read != count)
    {
        vdLogGlobalWarning("Read only '%u' out of '%u' ints!\n", 
        					(unsigned)read, (unsigned)count);
    }

    return read;
}

/* ============================================================================================== */

size_t
WriteRawBytes(FILE* fh, char* buffer, size_t count)
{
    if(fh == NULL || buffer == NULL || count < 1)
        return 0;

    size_t written = fwrite(buffer, sizeof(char), count, fh);

    if(written != count)
    {
        vdLogGlobalWarning("Wrote only '%u' out of '%u' bytes!\n", 
        				   (unsigned)written, (unsigned)count);
    }

    return written;
}

/* ============================================================================================== */

size_t
ReadRawBytes(FILE* fh, char* buffer, size_t count)
{
    if(fh == NULL || buffer == NULL || count < 1)
        return 0;

    size_t read = fread(buffer, sizeof(char), count, fh);

    if(read != count)
    {
        vdLogGlobalWarning("Read only '%u' out of '%u' bytes!\n", 
        				   (unsigned)read, (unsigned)count);
    }

    return read;
}

/* ============================================================================================== */



