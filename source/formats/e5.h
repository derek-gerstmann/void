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

#ifndef VD_DATA_E5_HEADER_INCLUDED
#define VD_DATA_E5_HEADER_INCLUDED

/* ============================================================================================== */

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================================== */

#include <stdlib.h>

/* ============================================================================================== */
#define H5_USE_16_API_DEFAULT 			(1)
#define E5_EMISSIVITY_GROUP_NAME_ABS    "/Emissivity"
#define E5_FLASH_GROUP_NAME_ABS         "/Flash"

#define E5_MAX_GROUP_NAME_LENGTH        (1024)
#define E5_MAX_ATTR_STRING_LENGTH       (80)

#define E5_TRUE                         (1)
#define E5_FALSE                        (0)
#define E5_DEFAULT                      (0)

#define E5_SUCCESS                       (1)
#define E5_UNKNOWN_FAILURE              (-1)
#define E5_READ_FAILURE                 (-2)
#define E5_WRITE_FAILURE                (-3)
#define E5_OUT_OF_MEMORY                (-4)
#define E5_INVALID_POINTER              (-6)
#define E5_INVALID_FILE                 (-7)
#define E5_INVALID_GROUP                (-8)
#define E5_INVALID_ATTRIBUTE            (-9)
#define E5_INVALID_DATASET              (-10)
#define E5_INVALID_TYPE                 (-11)

#define E5_NOTIFY_LEVEL_MAX              (10)
#define E5_NOTIFY_LEVEL_DEBUG		     (6)
#define E5_NOTIFY_LEVEL_TEST		     (5)
#define E5_NOTIFY_LEVEL_INFO		     (4)
#define E5_NOTIFY_LEVEL_WARNING		     (3)
#define E5_NOTIFY_LEVEL_ERROR            (2)
#define E5_NOTIFY_LEVEL_CRITICAL	     (1)
#define E5_NOTIFY_LEVEL_MIN              (0)

#define E5_EXIT_ON_ERROR(status) \
    if((status) < 0 || (status) != E5_SUCCESS) \
    { \
        fprintf(stderr, "ERROR: E5 command failed with error '%s' (%d) at [file %s, line %d]\n", \
                e5_error_string(status), (int)(status), __FILE__, __LINE__); \
        exit(EXIT_FAILURE); \
    }

/* ============================================================================================== */

typedef int                              eid_t;
typedef size_t                           esize_t;
typedef int                              estatus_t;
typedef int                              ebool_t;

/* ============================================================================================== */

typedef struct _e5_notify_info
{
    eid_t   object;
    int     level;
    int     status;
} e5_notify_info_t;

typedef void (*e5_notify_func)(const char * error_string, const e5_notify_info_t *notify_info);

typedef enum _e5_scale
{
    E5_VALUE_SCALE_INVALID = 0,
    E5_VALUE_SCALE_LINEAR,
    E5_VALUE_SCALE_LOG10,
    E5_VALUE_SCALE_UNKNOWN
} e5_scale;

typedef enum _e5_type
{
    E5_TYPE_INVALID = 0,
    E5_TYPE_INT,
    E5_TYPE_FLOAT,
    E5_TYPE_DOUBLE,
    E5_TYPE_FIXED_SIZE_STR,
    E5_TYPE_UNKNOWN
} e5_type;

typedef struct _e5_data_info
{
    const char* name;
    esize_t dim[3];
    int type;
    int scale;
} e5_data_info;

typedef struct _e5_scalar_attr
{
    const char* name;
    void* value;
    int type;
    size_t size;
} e5_attr;

typedef struct _e5_attr_int
{
    const char* name;
    int value;
} e5_attr_int;

typedef struct _e5_attr_float
{
    const char* name;
    float value;
} e5_attr_float;

typedef struct _e5_attr_double
{
    const char* name;
    double value;
} e5_attr_double;

typedef struct _e5_attr_str
{
    const char* name;
    char value[E5_MAX_ATTR_STRING_LENGTH];
} e5_attr_str;

typedef struct _e5_mutable_attr_int
{
    char* name;
    int value;
} e5_mutable_attr_int;

typedef struct _e5_mutable_attr_float
{
    char* name;
    float value;
} e5_mutable_attr_float;

typedef struct _e5_mutable_attr_double
{
    char* name;
    double value;
} e5_mutable_attr_double;

typedef struct _e5_mutable_attr_str
{
    char* name;
    char value[E5_MAX_ATTR_STRING_LENGTH];
} e5_mutable_attr_str;

typedef struct _e5_grid_dataset
{
    const char* name;
    void* data;
    esize_t dim[3];
    int type;
    int scale;
} e5_grid_dataset;

typedef struct _e5_mutable_grid_dataset
{
    char* name;
    void* data;
    esize_t dim[3];
    int type;
    int scale;
} e5_mutable_grid_dataset;

/* ============================================================================================== */

extern estatus_t
e5_set_notify_func(eid_t e5_object_id, e5_notify_func callback,
                   int min_notify_level, int max_notify_level);

extern esize_t
e5_sizeof(int type);

extern const char*
e5_typename(int type);

extern const char*
e5_basename(const char* name);

extern int
e5_success(estatus_t status);

extern const char*
e5_error_string(estatus_t status);

/* ============================================================================================== */

extern eid_t
e5_create_file(const char* e5_filename);

extern eid_t
e5_open_file(const char* e5_filename);

extern estatus_t
e5_merge_f5_file(eid_t e5_file_id, const char* f5_filename);

extern estatus_t
e5_close_file(eid_t e5_file_id);

/* ============================================================================================== */

extern eid_t
e5_create_group(eid_t e5_group_id, const char* group_name);

extern eid_t
e5_open_group(eid_t e5_group_id, const char* group_name);

extern estatus_t
e5_close_group(eid_t e5_group_id);

/* ============================================================================================== */

extern int
e5_is_valid_attr(eid_t e5_group_id, const char* attr_name);

extern estatus_t
e5_read_attr_list(eid_t e5_group_id, e5_attr* e5_scalars);

extern estatus_t
e5_read_attr(eid_t e5_group_id, const char* name, void* value, int* type, size_t* size);

extern estatus_t
e5_write_attr_list(eid_t e5_group_id, e5_attr* e5_scalars);

extern estatus_t
e5_write_attr(eid_t e5_group_id, const char* name, void* value, int type, size_t size);

/* ============================================================================================== */

extern estatus_t
e5_read_attr_list_int(eid_t e5_group_id, e5_attr_int* e5_int_scalars);

extern estatus_t
e5_read_attr_int(eid_t e5_group_id, const char* name, int* value);

extern estatus_t
e5_write_attr_list_int(eid_t e5_group_id, e5_attr_int* e5_int_scalars);

extern estatus_t
e5_write_attr_int(eid_t e5_group_id, const char* name, int value);

/* ============================================================================================== */

extern estatus_t
e5_read_attr_list_str(eid_t e5_group_id, e5_attr_str* e5_str_scalars);

extern estatus_t
e5_read_attr_str(eid_t e5_group_id, const char* name, char* value);

extern estatus_t
e5_write_attr_list_str(eid_t e5_group_id, e5_attr_str* e5_str_scalars);

extern estatus_t
e5_write_attr_str(eid_t e5_group_id, const char* name, char* value);

/* ============================================================================================== */

extern estatus_t
e5_read_attr_list_float(eid_t e5_group_id, e5_attr_float* e5_float_scalars);

extern estatus_t
e5_read_attr_float(eid_t e5_group_id, const char* name, float* value);

extern estatus_t
e5_write_attr_list_float(eid_t e5_group_id, e5_attr_float* e5_float_scalars);

extern estatus_t
e5_write_attr_float(eid_t e5_group_id, const char* name, float value);

/* ============================================================================================== */

extern estatus_t
e5_read_attr_list_double(eid_t e5_group_id, e5_attr_double* e5_double_scalars);

extern estatus_t
e5_read_attr_double(eid_t e5_group_id, const char* name, double* value);

extern estatus_t
e5_write_attr_list_double(eid_t e5_group_id, e5_attr_double* e5_double_scalars);

extern estatus_t
e5_write_attr_double(eid_t e5_group_id, const char* name, double value);

/* ============================================================================================== */

extern estatus_t
e5_read_grid_list(eid_t e5_group_id, const char* e5_list_name, e5_grid_dataset* grid_list);

extern estatus_t
e5_read_grid(eid_t e5_group_id, const char* e5_grid_name,
             void* data, esize_t* dim, int* data_type, int* value_scale);

extern estatus_t
e5_write_grid_list(eid_t e5_group_id, const char* e5_list_name, e5_grid_dataset* grid_list);

extern estatus_t
e5_write_grid(eid_t e5_group_id, const char* e5_grid_name,
              void* grid, esize_t* dims, int data_type, int value_scale);

/* ============================================================================================== */

extern estatus_t
e5_read_data_info(eid_t e5_group_id, const char* e5_grid_name,
                  esize_t* dim, int* data_type, int* value_scale);

extern estatus_t
e5_read_data_info_list(eid_t e5_group_id, const char* e5_list_name, e5_data_info* info_list);


/* ============================================================================================== */

#ifdef __cplusplus
}
#endif

/* ============================================================================================== */

#endif /* VD_DATA_E5_HEADER_INCLUDED */

/* ============================================================================================== */

#endif /* VD_USE_FLASH */


