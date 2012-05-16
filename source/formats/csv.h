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

#ifndef VD_FORMAT_CSV_HEADER_INCLUDED
#define VD_FORMAT_CSV_HEADER_INCLUDED

/* ============================================================================================== */

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================================== */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/* ============================================================================================== */

extern FILE*
OpenCsvFile(const char* filename, size_t *field_count);

extern void
CloseCsvFile(FILE *fh);

extern int 
IsCsvDelimiter( char c, const char* delims );

extern size_t 
ReadCsvWord(FILE* fp, const char* delims, char* buffer, size_t count);

extern float
ReadCsvFloatColumn(FILE* fh, size_t* count);
	
/* ============================================================================================== */

#ifdef __cplusplus
}
#endif

/* ============================================================================================== */

#endif /* VD_FORMAT_CSV_HEADER_INCLUDED */

/* ============================================================================================== */
