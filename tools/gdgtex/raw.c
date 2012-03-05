/***************************************************************************************************
//
// License:    The MIT License
//
// Copyright (c) 2011. Derek Gerstmann, The University of Western Australia.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
***************************************************************************************************/

#include "raw.h"

#ifdef __APPLE__
#include <libc.h>
#endif
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

/**************************************************************************************************/

FILE*
OpenReadOnlyRawFile(const char* filename, size_t* count)
{
    struct stat statbuf;
    FILE*        fh;

    fh = fopen(filename, "rb");

    if(fh == 0)
        return 0;

    stat(filename, &statbuf);
    size_t elements = statbuf.st_size / sizeof(float);
    *count = elements;
    return fh;
}

FILE*
OpenWriteOnlyRawFile(const char* filename)
{
    FILE*        fh;
    fh = fopen(filename, "wb+");

    if(fh == 0)
        return 0;

    return fh;
}

void
CloseRawFile(FILE* fh)
{
    fclose(fh);
}

size_t
WriteRawFloats(FILE* fh, float* buffer, size_t count)
{
    if(fh == NULL || buffer == NULL || count < 1)
        return 0;

    size_t written = fwrite(buffer, sizeof(float), count, fh);

    if(written != count)
    {
        printf("Warning: Wrote only '%u' out of '%u' floats!\n", (unsigned)written, (unsigned)count);
    }

    return written;
}

size_t
ReadRawFloats(FILE* fh, float* buffer, size_t count)
{
    if(fh == NULL || buffer == NULL || count < 1)
        return 0;

    size_t read = fread(buffer, sizeof(float), count, fh);

    if(read != count)
    {
        printf("Warning: Read only '%u' out of '%u' floats!\n", (unsigned)read, (unsigned)count);
    }

    return read;
}

size_t
WriteRawInts(FILE* fh, int* buffer, size_t count)
{
    if(fh == NULL || buffer == NULL || count < 1)
        return 0;

    size_t written = fwrite(buffer, sizeof(int), count, fh);

    if(written != count)
    {
        printf("Warning: Wrote only '%u' out of '%u' ints!\n", (unsigned)written, (unsigned)count);
    }

    return written;
}

size_t
ReadRawInts(FILE* fh, int* buffer, size_t count)
{
    if(fh == NULL || buffer == NULL || count < 1)
        return 0;

    size_t read = fread(buffer, sizeof(int), count, fh);

    if(read != count)
    {
        printf("Warning: Read only '%u' out of '%u' ints!\n", (unsigned)read, (unsigned)count);
    }

    return read;
}

size_t
WriteRawBytes(FILE* fh, char* buffer, size_t count)
{
    if(fh == NULL || buffer == NULL || count < 1)
        return 0;

    size_t written = fwrite(buffer, sizeof(char), count, fh);

    if(written != count)
    {
        printf("Warning: Wrote only '%u' out of '%u' chars!\n", (unsigned)written, (unsigned)count);
    }

    return written;
}

size_t
ReadRawBytes(FILE* fh, char* buffer, size_t count)
{
    if(fh == NULL || buffer == NULL || count < 1)
        return 0;

    size_t read = fread(buffer, sizeof(char), count, fh);

    if(read != count)
    {
        printf("Warning: Read only '%u' out of '%u' chars!\n", (unsigned)read, (unsigned)count);
    }

    return read;
}

/**************************************************************************************************/




