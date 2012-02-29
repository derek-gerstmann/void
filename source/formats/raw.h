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

#ifndef VD_DATA_RAW_HEADER_INCLUDED
#define VD_DATA_RAW_HEADER_INCLUDED

/* ============================================================================================== */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/* ============================================================================================== */

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================================== */

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

/* ============================================================================================== */

#ifdef __cplusplus
}
#endif

/* ============================================================================================== */

#endif

