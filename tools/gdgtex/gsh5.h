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

#ifndef VD_DATA_GSH5_HEADER_INCLUDED
#define VD_DATA_GSH5_HEADER_INCLUDED

/* ============================================================================================== */

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================================== */

#include <stdlib.h>

/* ============================================================================================== */

#define H5_USE_16_API_DEFAULT 			     (1)
#define VD_GSH5_MAX_GROUP_NAME_LENGTH        (1024)
#define VD_GSH5_MAX_ATTR_STRING_LENGTH       (80)

#define VD_GSH5_TRUE                         (1)
#define VD_GSH5_FALSE                        (0)
#define VD_GSH5_DEFAULT                      (0)

#define VD_GSH5_SUCCESS                       (1)
#define VD_GSH5_UNKNOWN_FAILURE              (-1)
#define VD_GSH5_READ_FAILURE                 (-2)
#define VD_GSH5_WRITE_FAILURE                (-3)
#define VD_GSH5_OUT_OF_MEMORY                (-4)
#define VD_GSH5_INVALID_POINTER              (-6)
#define VD_GSH5_INVALID_FILE                 (-7)
#define VD_GSH5_INVALID_GROUP                (-8)
#define VD_GSH5_INVALID_ATTRIBUTE            (-9)
#define VD_GSH5_INVALID_DATASET              (-10)
#define VD_GSH5_INVALID_TYPE                 (-11)

#define VD_GSH5_NOTIFY_LEVEL_MAX             (10)
#define VD_GSH5_NOTIFY_LEVEL_DEBUG		     (6)
#define VD_GSH5_NOTIFY_LEVEL_TEST		     (5)
#define VD_GSH5_NOTIFY_LEVEL_INFO		     (4)
#define VD_GSH5_NOTIFY_LEVEL_WARNING		 (3)
#define VD_GSH5_NOTIFY_LEVEL_ERROR           (2)
#define VD_GSH5_NOTIFY_LEVEL_CRITICAL	     (1)
#define VD_GSH5_NOTIFY_LEVEL_MIN             (0)

#define VD_GSH5_EXIT_ON_ERROR(status) \
    if((status) < 0 || (status) != VD_GSH5_SUCCESS) \
    { \
        fprintf(stderr, "ERROR: E5 command failed with error '%s' (%d) at [file %s, line %d]\n", \
                gsh5_error_string(status), (int)(status), __FILE__, __LINE__); \
        exit(EXIT_FAILURE); \
    }

/* ============================================================================================== */

typedef int                              vd_hid_t;
typedef size_t                           vd_size_t;
typedef int                              vd_status_t;
typedef int                              vd_bool_t;

/* ============================================================================================== */

typedef struct _gsh5_notify_info
{
    vd_hid_t            object;
    int                 level;
    int                 status;
} gsh5_notify_info_t;

typedef void (*gsh5_notify_func)(const char * error_string, const gsh5_notify_info_t *notify_info);

typedef enum _gsh5_scale
{
    VD_GSH5_VALUE_SCALE_INVALID = 0,
    VD_GSH5_VALUE_SCALE_LINEAR,
    VD_GSH5_VALUE_SCALE_LOG10,
    VD_GSH5_VALUE_SCALE_UNKNOWN
} gsh5_scale;

typedef enum _gsh5_type
{
    VD_GSH5_TYPE_INVALID = 0,
    VD_GSH5_TYPE_INT,
    VD_GSH5_TYPE_FLOAT,
    VD_GSH5_TYPE_DOUBLE,
    VD_GSH5_TYPE_FIXED_SIZE_STR,
    VD_GSH5_TYPE_UNKNOWN
} gsh5_type;

typedef struct _gsh5_data_info
{
    const char* name;
    vd_size_t dim[3];
    int type;
    int scale;
} gsh5_data_info;

typedef struct _gsh5_scalar_attr
{
    const char* name;
    void* value;
    int type;
    size_t size;
} gsh5_attr;

typedef struct _gsh5_attr_int
{
    const char* name;
    int value;
} gsh5_attr_int;

typedef struct _gsh5_attr_float
{
    const char* name;
    float value;
} gsh5_attr_float;

typedef struct _gsh5_attr_double
{
    const char* name;
    double value;
} gsh5_attr_double;

typedef struct _gsh5_attr_str
{
    const char* name;
    char value[VD_GSH5_MAX_ATTR_STRING_LENGTH];
} gsh5_attr_str;

typedef struct _gsh5_mutable_attr_int
{
    char* name;
    int value;
} gsh5_mutable_attr_int;

typedef struct _gsh5_mutable_attr_float
{
    char* name;
    float value;
} gsh5_mutable_attr_float;

typedef struct _gsh5_mutable_attr_double
{
    char* name;
    double value;
} gsh5_mutable_attr_double;

typedef struct _gsh5_mutable_attr_str
{
    char* name;
    char value[VD_GSH5_MAX_ATTR_STRING_LENGTH];
} gsh5_mutable_attr_str;

typedef struct _gsh5_grid_dataset
{
    const char* name;
    void* data;
    vd_size_t dim[3];
    int type;
    int scale;
} gsh5_grid_dataset;

typedef struct _gsh5_mutable_grid_dataset
{
    char* name;
    void* data;
    vd_size_t dim[3];
    int type;
    int scale;
} gsh5_mutable_grid_dataset;

/* ============================================================================================== */

extern vd_status_t
gsh5_set_notify_func(vd_hid_t gsh5_object_id, gsh5_notify_func callback,
                   int min_notify_level, int max_notify_level);

extern vd_size_t
gsh5_sizeof(int type);

extern const char*
gsh5_typename(int type);

extern const char*
gsh5_basename(const char* name);

extern int
gsh5_success(vd_status_t status);

extern const char*
gsh5_error_string(vd_status_t status);

/* ============================================================================================== */

extern vd_hid_t
gsh5_create_file(const char* gsh5_filename);

extern vd_hid_t
gsh5_open_file(const char* gsh5_filename);

extern vd_status_t
gsh5_merge_f5_file(vd_hid_t gsh5_file_id, const char* f5_filename);

extern vd_status_t
gsh5_close_file(vd_hid_t gsh5_file_id);

/* ============================================================================================== */

extern vd_hid_t
gsh5_create_group(vd_hid_t gsh5_group_id, const char* group_name);

extern vd_hid_t
gsh5_open_group(vd_hid_t gsh5_group_id, const char* group_name);

extern vd_status_t
gsh5_close_group(vd_hid_t gsh5_group_id);

/* ============================================================================================== */

extern int
gsh5_is_valid_attr(vd_hid_t gsh5_group_id, const char* attr_name);

extern vd_status_t
gsh5_read_attr_list(vd_hid_t gsh5_group_id, gsh5_attr* gsh5_scalars);

extern vd_status_t
gsh5_read_attr(vd_hid_t gsh5_group_id, const char* name, void* value, int* type, size_t* size);

extern vd_status_t
gsh5_write_attr_list(vd_hid_t gsh5_group_id, gsh5_attr* gsh5_scalars);

extern vd_status_t
gsh5_write_attr(vd_hid_t gsh5_group_id, const char* name, void* value, int type, size_t size);

/* ============================================================================================== */

extern vd_status_t
gsh5_read_attr_list_int(vd_hid_t gsh5_group_id, gsh5_attr_int* gsh5_int_scalars);

extern vd_status_t
gsh5_read_attr_int(vd_hid_t gsh5_group_id, const char* name, int* value);

extern vd_status_t
gsh5_write_attr_list_int(vd_hid_t gsh5_group_id, gsh5_attr_int* gsh5_int_scalars);

extern vd_status_t
gsh5_write_attr_int(vd_hid_t gsh5_group_id, const char* name, int value);

/* ============================================================================================== */

extern vd_status_t
gsh5_read_attr_list_str(vd_hid_t gsh5_group_id, gsh5_attr_str* gsh5_str_scalars);

extern vd_status_t
gsh5_read_attr_str(vd_hid_t gsh5_group_id, const char* name, char* value);

extern vd_status_t
gsh5_write_attr_list_str(vd_hid_t gsh5_group_id, gsh5_attr_str* gsh5_str_scalars);

extern vd_status_t
gsh5_write_attr_str(vd_hid_t gsh5_group_id, const char* name, char* value);

/* ============================================================================================== */

extern vd_status_t
gsh5_read_attr_list_float(vd_hid_t gsh5_group_id, gsh5_attr_float* gsh5_float_scalars);

extern vd_status_t
gsh5_read_attr_float(vd_hid_t gsh5_group_id, const char* name, float* value);

extern vd_status_t
gsh5_write_attr_list_float(vd_hid_t gsh5_group_id, gsh5_attr_float* gsh5_float_scalars);

extern vd_status_t
gsh5_write_attr_float(vd_hid_t gsh5_group_id, const char* name, float value);

/* ============================================================================================== */

extern vd_status_t
gsh5_read_attr_list_double(vd_hid_t gsh5_group_id, gsh5_attr_double* gsh5_double_scalars);

extern vd_status_t
gsh5_read_attr_double(vd_hid_t gsh5_group_id, const char* name, double* value);

extern vd_status_t
gsh5_write_attr_list_double(vd_hid_t gsh5_group_id, gsh5_attr_double* gsh5_double_scalars);

extern vd_status_t
gsh5_write_attr_double(vd_hid_t gsh5_group_id, const char* name, double value);

/* ============================================================================================== */

extern vd_status_t
gsh5_read_grid_list(
    vd_hid_t gsh5_group_id, const char* gsh5_list_name, gsh5_grid_dataset* grid_list);

extern vd_status_t
gsh5_read_grid(
    vd_hid_t gsh5_group_id, const char* gsh5_grid_name,
    void* data, vd_size_t* dim, int* data_type, int* value_scale);

extern vd_status_t
gsh5_write_grid_list(
    vd_hid_t gsh5_group_id, const char* gsh5_list_name, 
    gsh5_grid_dataset* grid_list);

extern vd_status_t
gsh5_write_grid(
    vd_hid_t gsh5_group_id, const char* gsh5_grid_name,
    void* grid, vd_size_t* dims, int data_type, int value_scale);

/* ============================================================================================== */

extern vd_status_t
gsh5_read_data_info(
    vd_hid_t gsh5_group_id, const char* gsh5_grid_name, 
    vd_size_t* dim, int* data_type, int* value_scale);

extern vd_status_t
gsh5_read_data_info_list(
    vd_hid_t gsh5_group_id, const char* gsh5_list_name, 
    gsh5_data_info* info_list);


/* ============================================================================================== */

#ifdef __cplusplus
}
#endif

/* ============================================================================================== */

#endif /* VD_DATA_GSH5_HEADER_INCLUDED */

