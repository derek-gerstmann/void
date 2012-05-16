/* ================================================================================================
//
// License:     The Lesser GNU Public License (LGPL) v3.0.
// 
// Author(s):   Derek Gerstmann. The University of Western Australia (UWA). 
//              As well as the shoulders of many giants...
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
  =============================================================================================== */

#include "formats/csv.h"

/* ============================================================================================== */

#include <libc.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

/* ============================================================================================== */

enum { CSV_DEFAULT_BUFFER_SIZE = 1024 };

/* ============================================================================================== */

FILE*
OpenCsvFile(const char* filename, size_t *field_count)
{
	const size_t buffer_size = CSV_DEFAULT_BUFFER_SIZE;
	char buffer[CSV_DEFAULT_BUFFER_SIZE] = {0};
	size_t lengths[CSV_DEFAULT_BUFFER_SIZE] = {0};
	
    struct stat statbuf;
    FILE        *fh;

    fh = fopen(filename, "r");
    if (fh == 0)
        return 0;

    stat(filename, &statbuf);
	size_t size = (size_t)statbuf.st_size;
	size = size < buffer_size ? size : buffer_size;
    size_t read = fread(buffer, size, 1, fh);
	fclose(fh);

	char* text = buffer;
	const char *sep = ", ";
	char *field, *delim;

	size_t column = 0;
	size_t offset = 0;
	for (field = strtok_r(text, sep, &delim); field; field = strtok_r(NULL, sep, &delim))
	{
		char* newline = strstr(field, "\n");
		if(newline) newline[0] = '\0';

		lengths[column] = strlen(field);
		column++;
		
		if(newline) 
		{
			field = &field[1];
			offset = field - text;
			lengths[column] = strlen(field);
			if(lengths[column]) column++;
			break;
		}
	}
	(*field_count) = column;

    fh = fopen(filename, "r");
//    read = fread(buffer, offset+1, 1, fh);
//    buffer[offset] = '\0';
    // printf("'%04d' columns in '%s' first line\n", column, buffer);
    return fh;	
}

/* ============================================================================================== */

void
CloseCsvFile(FILE *fh)
{
	fclose(fh);
}

/* ============================================================================================== */

int 
IsCsvDelimiter( char c, const char* delims )
{
	size_t i;
	for(i = 0; i < strlen(delims); ++i)
		if(c == delims[i])
			return 1;
	
	return 0;
}

/* ============================================================================================== */

size_t 
ReadCsvWord(FILE* fp, const char* delims, char* buffer, size_t count) 
{
    size_t n;
    char c;
	int skipped = 0;
	
    if (count < 1)
        return -1;

    n = 0;
    c = fgetc( fp );
    while(c != EOF && IsCsvDelimiter(c, delims))
    {
    	skipped++;
	    c = fgetc(fp);
    }
    	
    while( c != EOF && !IsCsvDelimiter( c, delims ) && n < count ) {
        buffer[ n ] = c;
        ++n;
        c = fgetc( fp );
    }

    if( n < count ) {
        buffer[ n ] = '\0';
        return n;
    }

    return -1;
}

/* ============================================================================================== */

float
ReadCsvFloatColumn(FILE* fh, size_t* count)
{
	char buffer[4096] = {0};
    float value = 0.0f;

	size_t length = ReadCsvWord(fh, ", \n\r\0", buffer, 4096);
    (*count) = length;
	if(length)
	{
		value = (float)strtod(buffer, NULL);
	}
	return value;
}

/* ============================================================================================== */

