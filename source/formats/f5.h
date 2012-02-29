/* ================================================================================================
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
  =============================================================================================== */

#if defined(VD_USE_FLASH)

/* ============================================================================================== */

#ifndef VD_DATA_F5_HEADER_INCLUDED
#define VD_DATA_F5_HEADER_INCLUDED

/* ============================================================================================== */

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================================== */

#define F5_FILE_FORMAT_VERSION      (9)
#define F5_MAX_STRING_LENGTH        (80)
#define F5_LIST_STRING_SIZE         F5_MAX_STRING_LENGTH

/* ============================================================================================== */

typedef struct int_list_t
{
    char name[F5_LIST_STRING_SIZE];
    int value;
} f5_int_list_t;

typedef struct real_list_t
{
    char name[F5_LIST_STRING_SIZE];
    double value;
} f5_real_list_t;

typedef struct str_list_t
{
    char value[F5_LIST_STRING_SIZE];
    char name[F5_LIST_STRING_SIZE];
} f5_str_list_t;

typedef struct log_list_t
{
    int value;
    char name[F5_LIST_STRING_SIZE];
} f5_log_list_t;

/* ============================================================================================== */

#ifdef __cplusplus
}
#endif

/* ============================================================================================== */

#endif /* VD_DATA_F5_HEADER_INCLUDED */

/* ============================================================================================== */

#endif /* VD_USE_FLASH */


