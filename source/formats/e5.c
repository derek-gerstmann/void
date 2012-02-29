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

#include "e5.h"
#include "f5.h"
#include "hdf5.h"

/* ============================================================================================== */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stddef.h>
#include <string.h>

/* ============================================================================================== */

static estatus_t
e5_notify(eid_t e5_object_id, int level, eid_t status, const char* format, ...);

#define E5_NOTIFY_WHERESTR              "[file %s, line %d]: "
#define E5_NOTIFY_WHEREARG              __FILE__, __LINE__

#define E5_NOTIFY_PREFIX_TEST			"[test]"
#define E5_NOTIFY_PREFIX_DEBUG			"[debug]"
#define E5_NOTIFY_PREFIX_INFO			"[info]"
#define E5_NOTIFY_PREFIX_WARNING		"[warning]"
#define E5_NOTIFY_PREFIX_CRITICAL		"[critical]"
#define E5_NOTIFY_PREFIX_ERROR			"[error]"
#define E5_DEFAULT_NOTIFY_HANDLERS      (32)

#define E5_NOTIFY(_eid, _level, _status, ... ) \
    e5_notify( (_eid), (_level), (_status), __VA_ARGS__ )

#define e5_debug(_eid, _fmt, ...) \
    E5_NOTIFY(_eid, E5_NOTIFY_LEVEL_DEBUG, 0, E5_NOTIFY_WHERESTR _fmt, E5_NOTIFY_WHEREARG, __VA_ARGS__)

#define e5_info(_eid, _fmt, ...)  \
    E5_NOTIFY(_eid, E5_NOTIFY_LEVEL_INFO, 0, _fmt, __VA_ARGS__)

#define e5_error(_eid, _status, _fmt, ...) \
    E5_NOTIFY(_eid, E5_NOTIFY_LEVEL_ERROR, _status, E5_NOTIFY_WHERESTR _fmt, E5_NOTIFY_WHEREARG, __VA_ARGS__)

#define e5_critical(_eid, _status, _fmt, ...) \
    E5_NOTIFY(_eid, E5_NOTIFY_LEVEL_CRITICAL, _status, E5_NOTIFY_WHERESTR _fmt, E5_NOTIFY_WHEREARG, __VA_ARGS__)

/* ============================================================================================== */

static e5_notify_func E5_GLOBAL_NOTIFY_FUNC = 0;
static int E5_GLOBAL_NOTIFY_MIN_LEVEL = 0;
static int E5_GLOBAL_NOTIFY_MAX_LEVEL = 10;

/* ============================================================================================== */

typedef struct _eid_t
{
    eid_t	        object;
    e5_notify_func  notify;
} eid_internal_t;

typedef enum _f5_scalar_list_type
{
    F5_SCALAR_LIST_INVALID = 0,
    F5_SCALAR_LIST_REAL,
    F5_SCALAR_LIST_INTEGER,
    F5_SCALAR_LIST_STRING,
    F5_SCALAR_LIST_LOGICAL,
    F5_SCALAR_LIST_UNKNOWN
} f5_scalar_list_type;

static ebool_t
is_whitespace(char c)
{
    switch (c)
    {
    case ' ':
    case '\n':
    case '\t':
    case '\f':
    case '\r':
        return E5_TRUE;
        break;
    default:
        return E5_FALSE;
        break;
    }
    return E5_FALSE;
}

static void*
e5_malloc(size_t bytes)
{
    void* ptr = malloc(bytes);
#ifdef E5_DEBUG
    E5_EXIT_ON_ERROR((ptr == NULL) ? (-1) : (E5_SUCCESS));
#endif
    return ptr;
}

static void
e5_free(void* ptr)
{
#ifdef E5_DEBUG
    E5_EXIT_ON_ERROR((ptr == NULL) ? (-1) : (E5_SUCCESS));
#endif
    free(ptr);
}

static char*
e5_trim(
    char* input, int left, int right, size_t maxlen)
{
    size_t n = 0;
    char* start = input;
    while (left && is_whitespace(*start))
        start++;

    n = start - input;
    char* ptr = start;
    char* end = start;
    while (right && *ptr++ != '\0' && n++ < (maxlen-1))
    {
        if (!is_whitespace(*ptr))
            end = ptr;
    }

    *(++end) = '\0';
    return start;
}

int
e5_success(estatus_t s)
{
    return ((s == E5_SUCCESS) ? E5_TRUE : E5_FALSE);
}

const char*
e5_error_string(
    estatus_t status)
{
    int value = (int)status;
    switch(value)
    {
        case E5_SUCCESS:
            return "\0";
        case E5_READ_FAILURE:
            return "read failure";
        case E5_WRITE_FAILURE:
            return "write failure";
        case E5_OUT_OF_MEMORY:
            return "out-of-memory";
        case E5_INVALID_POINTER:
            return "invalid pointer";
        case E5_INVALID_FILE:
            return "invalid file";
        case E5_INVALID_GROUP:
            return "invalid group";
        case E5_INVALID_ATTRIBUTE:
            return "invalid attribute";
        case E5_INVALID_DATASET:
            return "invalid dataset";
        case E5_INVALID_TYPE:
            return "invalid type";
        case E5_UNKNOWN_FAILURE:
        default:
            return "unknown";
    }
    return "unknown";
}

esize_t
e5_sizeof(
    int type)
{
    switch(type)
    {
    case E5_TYPE_INT:
        return sizeof(int);
    case E5_TYPE_FLOAT:
        return sizeof(float);
    case E5_TYPE_DOUBLE:
        return sizeof(double);
    case E5_TYPE_FIXED_SIZE_STR:
        return sizeof(char) * E5_MAX_ATTR_STRING_LENGTH;
    default:
        return 0;
    };

    return 0;
}

const char*
e5_typename(
    int type)
{
    switch(type)
    {
    case E5_TYPE_INT:
        return "int";
    case E5_TYPE_FLOAT:
        return "float";
    case E5_TYPE_DOUBLE:
        return "double";
    case E5_TYPE_FIXED_SIZE_STR:
        return "str";
    default:
        return 0;
    };

    return 0;
}

static ebool_t
e5_is_valid_type(
    int type)
{
    switch(type)
    {
    case E5_TYPE_INT:
        return E5_TRUE;
    case E5_TYPE_FLOAT:
        return E5_TRUE;
    case E5_TYPE_DOUBLE:
        return E5_TRUE;
    case E5_TYPE_FIXED_SIZE_STR:
        return E5_TRUE;
    case E5_TYPE_INVALID:
    case E5_TYPE_UNKNOWN:
    default:
        return E5_FALSE;
    };

    return E5_FALSE;
}

static hid_t
e5_get_native_h5_type(hid_t type)
{
    hid_t p_type;
    H5T_class_t type_class;

    type_class = H5Tget_class(type);
    if(type_class==H5T_BITFIELD)
        p_type=H5Tcopy(type);
    else
        p_type = H5Tget_native_type(type,H5T_DIR_DEFAULT);

    return(p_type);
}

#if 0
static char*
e5_get_group_name(const char* name)
{
    int j;
    char *group_name;

    j = (int)strlen(name) - 1;
    group_name = (char *)e5_malloc((size_t)j + 2);

    while(name[j] != '/' && j >= 0)
        j--;

    if(j == -1)
    {
        strcpy(group_name, "/");
    }
    else
    {
        strncpy(group_name, name, (size_t)j + 1);
        group_name[j + 1] = '\0';
    }

    return group_name;
}
#endif

const char*
e5_basename(const char* name)
{
    int j;
    const char *base_name;

    j = (int)strlen(name) - 1;
    while(name[j] != '/' && j >= 0)
        j--;

    base_name = name + j + 1;
    return base_name;
}

/* ============================================================================================== */

static estatus_t
e5_notify_handler(
    eid_t e5_object_id,
    int level,
    eid_t status,
    const char* format,
    va_list arg_list)
{
    static char buffer[4096] = {0};
    const char *prefix = 0;
    e5_notify_info_t info;

    if(E5_GLOBAL_NOTIFY_FUNC == 0)
        return E5_SUCCESS;

    if(level < E5_GLOBAL_NOTIFY_MIN_LEVEL)
        return E5_SUCCESS;

    if(level > E5_GLOBAL_NOTIFY_MAX_LEVEL)
        return E5_SUCCESS;

    switch(level)
    {
    case E5_NOTIFY_LEVEL_DEBUG:
    {
        prefix = E5_NOTIFY_PREFIX_DEBUG;
        break;
    }
    case E5_NOTIFY_LEVEL_TEST:
    {
        prefix = E5_NOTIFY_PREFIX_TEST;
        break;
    }
    case E5_NOTIFY_LEVEL_INFO:
    {
        prefix = E5_NOTIFY_PREFIX_INFO;
        break;
    }
    case E5_NOTIFY_LEVEL_WARNING:
    {
        prefix = E5_NOTIFY_PREFIX_WARNING;
        break;
    }
    case E5_NOTIFY_LEVEL_CRITICAL:
    {
        prefix = E5_NOTIFY_PREFIX_CRITICAL;
        break;
    }
    case E5_NOTIFY_LEVEL_ERROR:
    {
        prefix = E5_NOTIFY_PREFIX_ERROR;
        break;
    }
    default:
    {
        prefix = E5_NOTIFY_PREFIX_INFO;
        break;
    }
    };

    (void)vsnprintf(buffer, sizeof(buffer), format, arg_list);

    info.level = level;
    info.status = status;
    info.object = e5_object_id;

    E5_GLOBAL_NOTIFY_FUNC(buffer, &info);

    return E5_SUCCESS;
}

static estatus_t
e5_notify(
    eid_t e5_object_id, int level, eid_t status, const char* format, ...)
{
    va_list arg_list;
    va_start(arg_list, format);
    status = e5_notify_handler(e5_object_id, level, status, format, arg_list);
    va_end(arg_list);
    return status;
}

/* ============================================================================================== */

static estatus_t
e5_merge_flash_scalars(
    eid_t e5_file_id, hid_t f5_file_id,
    const char* f5_list_name, f5_scalar_list_type f5_list_type)
{
    size_t s;
    hsize_t dimens_1d;
    hsize_t maxdimens_1d;
    hid_t string_type;

    hsize_t f5_scalar_bytes;
    hid_t f5_dataspace;
    hid_t f5_memspace;
    hid_t f5_dataset;
    hid_t f5_type;
    void* f5_data;

    hid_t e5_em_group_id;
    int hstatus;
    size_t e5_scalar_bytes;

    estatus_t estatus = E5_SUCCESS;

    string_type = H5Tcopy(H5T_C_S1);
    H5Tset_size(string_type, F5_MAX_STRING_LENGTH);

    f5_dataset = H5Dopen(f5_file_id, f5_list_name);
    if (f5_dataset < 0)
    {
        estatus = E5_INVALID_DATASET;
        e5_error(f5_file_id, estatus, "Failed to open dataset '%s'\n", f5_list_name);
        return estatus;
    }

    f5_dataspace = H5Dget_space(f5_dataset);
    H5Sget_simple_extent_dims(f5_dataspace, &dimens_1d, &maxdimens_1d);

    e5_em_group_id = e5_open_group(e5_file_id, E5_EMISSIVITY_GROUP_NAME_ABS);

    switch(f5_list_type)
    {
    case F5_SCALAR_LIST_INTEGER:
    case F5_SCALAR_LIST_LOGICAL:
    {
        hid_t f5_int_list_type;
        f5_int_list_t *f5_int_list;

        f5_scalar_bytes = dimens_1d * sizeof(f5_int_list_t);
        f5_int_list = (f5_int_list_t *) e5_malloc(f5_scalar_bytes);
        if(!f5_int_list)
        {
            estatus = E5_OUT_OF_MEMORY;
            e5_error(f5_file_id, estatus, "Failed to allocate memory for reading '%s' from F5 data file\n", f5_list_name);
            break;
        }

        memset(f5_int_list, 0, f5_scalar_bytes);
        f5_int_list_type = H5Tcreate(H5T_COMPOUND, sizeof(f5_int_list_t));
        H5Tinsert(f5_int_list_type, "name",  HOFFSET(f5_int_list_t, name), string_type);
        H5Tinsert(f5_int_list_type, "value", HOFFSET(f5_int_list_t, value), H5T_NATIVE_INT);

        f5_memspace = H5Screate_simple(1, &dimens_1d, NULL);
        hstatus = H5Dread(f5_dataset, f5_int_list_type, f5_memspace, f5_dataspace, H5P_DEFAULT, f5_int_list);
        if (hstatus < 0)
        {
            estatus = E5_READ_FAILURE;
            e5_error(f5_file_id, estatus, "Failed to read '%s' dataset from data file\n", f5_list_name);
            break;
        }
        f5_data = f5_int_list;
        f5_type = f5_int_list_type;

        if(f5_data)
        {
            // Add the scalar list as attributes for the top-level Emissivity group
            e5_scalar_bytes = (dimens_1d + 1) * sizeof(e5_mutable_attr_int);
            e5_mutable_attr_int* e5_int_scalars = (e5_mutable_attr_int*) e5_malloc(e5_scalar_bytes);
            memset(e5_int_scalars, 0, e5_scalar_bytes);
            for(s = 0; s < dimens_1d; s++)
            {
                e5_int_scalars[s].value = f5_int_list[s].value;
                e5_int_scalars[s].name = e5_trim(f5_int_list[s].name, E5_TRUE, E5_TRUE, F5_MAX_STRING_LENGTH);
            }
            e5_write_attr_list_int(e5_em_group_id, (e5_attr_int*)e5_int_scalars);
            e5_free(e5_int_scalars);
        }
        break;
    }
    case F5_SCALAR_LIST_REAL:
    {
        hid_t f5_real_list_type;
        f5_real_list_t *f5_real_list;

        f5_scalar_bytes = dimens_1d * sizeof(f5_real_list_t);
        f5_real_list = (f5_real_list_t *) e5_malloc(f5_scalar_bytes);
        if(!f5_real_list)
        {
            estatus = E5_OUT_OF_MEMORY;
            e5_error(f5_file_id, estatus, "Failed to allocate memory for reading '%s' from F5 data file\n", f5_list_name);
            break;
        }

        memset(f5_real_list, 0, f5_scalar_bytes);
        f5_real_list_type = H5Tcreate(H5T_COMPOUND, sizeof(f5_real_list_t));
        H5Tinsert(f5_real_list_type, "name",  HOFFSET(f5_real_list_t, name), string_type);
        H5Tinsert(f5_real_list_type, "value", HOFFSET(f5_real_list_t, value), H5T_NATIVE_DOUBLE);

        f5_memspace = H5Screate_simple(1, &dimens_1d, NULL);
        hstatus = H5Dread(f5_dataset, f5_real_list_type, f5_memspace, f5_dataspace, H5P_DEFAULT, f5_real_list);
        if (hstatus < 0)
        {
            estatus = E5_READ_FAILURE;
            e5_error(f5_file_id, estatus, "Failed to read '%s' from F5 data file\n", f5_list_name);
            break;
        }

        f5_data = f5_real_list;
        f5_type = f5_real_list_type;

        if(f5_data)
        {
            // Add the scalar list as attributes for the top-level Emissivity group
            e5_scalar_bytes = (dimens_1d + 1) * sizeof(e5_mutable_attr_double);
            e5_mutable_attr_double* e5_double_scalars = (e5_mutable_attr_double*) e5_malloc(e5_scalar_bytes);
            memset(e5_double_scalars, 0, e5_scalar_bytes);

            for(s = 0; s < dimens_1d; s++)
            {
                e5_double_scalars[s].value = f5_real_list[s].value;
                e5_double_scalars[s].name = f5_real_list[s].name;
                e5_double_scalars[s].name = e5_trim(f5_real_list[s].name, E5_TRUE, E5_TRUE, F5_MAX_STRING_LENGTH);
            }

            e5_write_attr_list_double(e5_em_group_id, (e5_attr_double*)e5_double_scalars);
            e5_free(e5_double_scalars);
        }
        break;
    }
    case F5_SCALAR_LIST_STRING:
    {
        hid_t f5_str_list_type;
        f5_str_list_t *f5_str_list;

        f5_scalar_bytes = dimens_1d * sizeof(f5_str_list_t);
        f5_str_list = (f5_str_list_t *) e5_malloc(f5_scalar_bytes);
        if(!f5_str_list)
        {
            estatus = E5_OUT_OF_MEMORY;
            e5_error(f5_file_id, estatus, "Failed to allocate memory for reading '%s' from F5 data file\n", f5_list_name);
            break;
        }

        memset(f5_str_list, 0, f5_scalar_bytes);
        f5_str_list_type = H5Tcreate(H5T_COMPOUND, sizeof(f5_str_list_t));
        H5Tinsert(f5_str_list_type, "name",  HOFFSET(f5_str_list_t, name), string_type);
        H5Tinsert(f5_str_list_type, "value", HOFFSET(f5_str_list_t, value), string_type);

        f5_memspace = H5Screate_simple(1, &dimens_1d, NULL);
        hstatus = H5Dread(f5_dataset, f5_str_list_type, f5_memspace, f5_dataspace, H5P_DEFAULT, f5_str_list);
        if (hstatus < 0)
        {
            estatus = E5_READ_FAILURE;
            e5_error(f5_file_id, estatus, "Failed to read '%s' from F5 data file\n", f5_list_name);
            break;
        }

        f5_data = f5_str_list;
        f5_type = f5_str_list_type;

        if(f5_data)
        {
            // Add the scalar list as attributes for the top-level Emissivity group
            e5_scalar_bytes = (dimens_1d + 1) * sizeof(e5_mutable_attr_str);
            e5_mutable_attr_str* e5_str_scalars = (e5_mutable_attr_str*) e5_malloc(e5_scalar_bytes);
            memset(e5_str_scalars, 0, e5_scalar_bytes);

            for(s = 0; s < dimens_1d; s++)
            {
                char* trimmed = e5_trim(f5_str_list[s].value, E5_TRUE, E5_TRUE, E5_MAX_ATTR_STRING_LENGTH);
                strncpy( e5_str_scalars[s].value, trimmed, E5_MAX_ATTR_STRING_LENGTH);
                e5_str_scalars[s].name = e5_trim(f5_str_list[s].name, E5_TRUE, E5_TRUE, F5_MAX_STRING_LENGTH);
            }

            e5_write_attr_list_str(e5_em_group_id, (e5_attr_str*)e5_str_scalars);
            e5_free(e5_str_scalars);
        }
        break;
    }
    default:
    {
        estatus = E5_INVALID_DATASET;
        e5_error(f5_file_id, estatus, "Unknown scalar list requested '%s' from F5 data file\n", f5_list_name);
        break;
    }
    };

    H5Tclose(f5_type);
    H5Sclose(f5_memspace);
    H5Sclose(f5_dataspace);
    H5Dclose(f5_dataset);

    e5_free(f5_data);
    e5_close_group(e5_em_group_id);

    return estatus;
}

/* ============================================================================================== */

static hid_t
e5_convert_type_to_hdf(int type)
{
    switch( type )
    {
    case E5_TYPE_INT:
        return H5T_STD_I32LE;
    case E5_TYPE_FLOAT:
        return H5T_IEEE_F32LE;
    case E5_TYPE_DOUBLE:
        return H5T_IEEE_F64LE;
    case E5_TYPE_FIXED_SIZE_STR:
        return H5T_C_S1;
    case E5_TYPE_INVALID:
    case E5_TYPE_UNKNOWN:
    default:
        return 0;
    }
    return 0;
}

static int
e5_convert_hdf_type(hid_t e5_type_id)
{
    if(H5Tequal(e5_type_id, H5T_STD_I32LE))
        return E5_TYPE_INT;
    else if(H5Tequal(e5_type_id, H5T_NATIVE_INT))
        return E5_TYPE_INT;
    else if(H5Tequal(e5_type_id, H5T_IEEE_F32LE))
        return E5_TYPE_FLOAT;
    else if(H5Tequal(e5_type_id, H5T_NATIVE_FLOAT))
        return E5_TYPE_FLOAT;
    else if(H5Tequal(e5_type_id, H5T_IEEE_F64LE))
        return E5_TYPE_DOUBLE;
    else if(H5Tequal(e5_type_id, H5T_NATIVE_DOUBLE))
        return E5_TYPE_DOUBLE;
    else if(H5Tequal(e5_type_id, H5T_C_S1 ))
        return E5_TYPE_FIXED_SIZE_STR;
    else
        return E5_TYPE_INVALID;

    return E5_TYPE_INVALID;
}

static hid_t
e5_convert_format_to_hdf(int type)
{
    switch( type )
    {
    case E5_TYPE_INT:
        return H5T_NATIVE_INT;
    case E5_TYPE_FLOAT:
        return H5T_NATIVE_FLOAT;
    case E5_TYPE_DOUBLE:
        return H5T_NATIVE_DOUBLE;
    case E5_TYPE_FIXED_SIZE_STR:
        return H5T_C_S1;
    case E5_TYPE_INVALID:
    case E5_TYPE_UNKNOWN:
    default:
        return 0;
    }
    return 0;

}

/* ============================================================================================== */

estatus_t
e5_read_attr_list(
    hid_t e5_group_id, e5_attr* e5_attr_list)
{
    int i;
    estatus_t status = E5_SUCCESS;

    hid_t h5_type;
    hid_t h5_native_type;
    hid_t h5_space;

    hid_t e5_attribute_id;

    for(i = 0; e5_attr_list && e5_attr_list[i].name != 0; i++)
    {
        e5_attr *attr = &e5_attr_list[i];
        if(attr->name == 0 || strlen(attr->name) < 1)
            continue;

        e5_info(e5_group_id, "Reading attribute [name='%s']\n", attr->name);

        if(H5Aexists(e5_group_id, attr->name) <= 0)
        {
            status = E5_INVALID_ATTRIBUTE;
            e5_error(e5_group_id, status, "Invalid name requested for attribute [type='%d', name='%s', value='%p']\n",  attr->type, attr->name, attr->value);
            continue;
        }

        e5_attribute_id = H5Aopen(e5_group_id, attr->name, H5P_DEFAULT);
        h5_type = H5Aget_type(e5_attribute_id);
        h5_space = H5Aget_space(e5_attribute_id);
        h5_native_type = e5_get_native_h5_type(h5_type);

        attr->type = e5_convert_hdf_type(h5_type);
        if(e5_is_valid_type(attr->type) != E5_TRUE)
        {
            status = E5_INVALID_TYPE;
            e5_error(e5_group_id, status, "Invalid type requested for attribute [type='%d', name='%s', value='%p']\n",  attr->type, attr->name, attr->value);
            continue;
        }

        H5Aread(e5_attribute_id, h5_native_type, attr->value);
        H5Aclose(e5_attribute_id);

        e5_info(e5_group_id, "Read attribute [type='%s', name='%s', value='%p']\n",
            e5_typename(attr->type), attr->name, attr->value);
    }
    return status;
}

estatus_t
e5_write_attr_list(
    hid_t e5_group_id, e5_attr* e5_attr_list)
{
    int i;
    eid_t status = E5_SUCCESS;
    hid_t e5_attribute_id;
    hid_t e5_dataspace_id = H5Screate(H5S_SCALAR);
    for(i = 0; e5_attr_list && e5_attr_list[i].name != 0; i++)
    {
        e5_attr *attr = &e5_attr_list[i];
        if(attr->name == 0 || strlen(attr->name) < 1)
            continue;

        if(e5_is_valid_type(e5_attr_list[i].type) != E5_TRUE)
        {
            status = E5_INVALID_TYPE;
            e5_error(e5_group_id, status, "Invalid type requested for attribute [type='%d', name='%s', value='%p']\n",  attr->type, attr->name, attr->value);
            continue;
        }

        hid_t hdf_type = e5_convert_type_to_hdf(e5_attr_list[i].type);
        hid_t hdf_format = e5_convert_format_to_hdf(e5_attr_list[i].type);

        if(H5Aexists(e5_group_id, attr->name) <= 0)
            e5_attribute_id = H5Acreate(e5_group_id, attr->name, hdf_type, e5_dataspace_id, H5P_DEFAULT);

        e5_info(e5_group_id, "Adding attribute [type='integer', name='%s', value='%d']\n",  attr->name, attr->value);
        e5_attribute_id = H5Aopen(e5_group_id, attr->name, H5P_DEFAULT);
        H5Awrite(e5_attribute_id, hdf_format, &(attr->value));
        H5Aclose(e5_attribute_id);
    }
    H5Sclose(e5_dataspace_id);
    return E5_SUCCESS;
}

int
e5_is_valid_attr(
    eid_t e5_group_id, const char* attr_name)
{
    if(H5Aexists(e5_group_id, attr_name) <= 0)
        return E5_FALSE;
    return E5_TRUE;
}

estatus_t
e5_read_attr(
    eid_t e5_group_id, const char* attr_name, void* value, int* type, size_t* size)
{
    e5_attr attr_list[] =
    {
        { attr_name, value, 0, 0,  },
        { 0 }
    };

    estatus_t status = e5_read_attr_list(e5_group_id, attr_list);
    if(e5_success(status) == E5_TRUE)
    {
        if(type) *type = attr_list[0].type;
        if(size) *size = attr_list[0].size;
        return E5_SUCCESS;
    }
    return status;
}

/* ============================================================================================== */

estatus_t
e5_read_attr_list_int(
    hid_t e5_group_id, e5_attr_int* e5_attr_list)
{
    int i;
    hid_t e5_attribute_id;
    estatus_t status = E5_SUCCESS;

    for(i = 0; e5_attr_list && e5_attr_list[i].name != 0; i++)
    {
        e5_attr_int *attr = &e5_attr_list[i];
        if(attr->name == 0 || strlen(attr->name) < 1)
            continue;

        e5_info(e5_group_id, "Reading attribute [type='integer', name='%s']\n",  attr->name);

        if(H5Aexists(e5_group_id, attr->name) <= 0)
        {
            status = E5_READ_FAILURE;
            e5_error(e5_group_id, status, "Specified attribute '%s' does not exist\n", attr->name);
            continue;
        }

        e5_attribute_id = H5Aopen(e5_group_id, attr->name, H5P_DEFAULT);
        H5Aread(e5_attribute_id, H5T_NATIVE_INT, &(attr->value));
        H5Aclose(e5_attribute_id);

        e5_info(e5_group_id, "Read attribute [type='integer', name='%s', value='%d']\n",  attr->name, attr->value);
    }
    return status;
}

estatus_t
e5_read_attr_int(
    hid_t e5_group_id, const char* name, int* value)
{
    e5_attr_int attr[] =
    {
        { name, 0 },
        { 0, 0 }
    };

    estatus_t status = e5_read_attr_list_int(e5_group_id, attr);

    if(value)
        *value = attr[0].value;

    return status;
}

estatus_t
e5_write_attr_list_int(
    hid_t e5_group_id, e5_attr_int* e5_attr_list)
{
    int i;
    hid_t e5_attribute_id;
    hid_t e5_dataspace_id = H5Screate(H5S_SCALAR);
    for(i = 0; e5_attr_list && e5_attr_list[i].name != 0; i++)
    {
        e5_attr_int *attr = &e5_attr_list[i];
        if(attr->name == 0 || strlen(attr->name) < 1)
            continue;

        if(H5Aexists(e5_group_id, attr->name) <= 0)
            e5_attribute_id = H5Acreate(e5_group_id, attr->name, H5T_STD_I32LE, e5_dataspace_id, H5P_DEFAULT);

        e5_info(e5_group_id, "Adding attribute [type='integer', name='%s', value='%d']\n",  attr->name, attr->value);
        e5_attribute_id = H5Aopen(e5_group_id, attr->name, H5P_DEFAULT);
        H5Awrite(e5_attribute_id, H5T_NATIVE_INT, &(attr->value));
        H5Aclose(e5_attribute_id);
    }
    H5Sclose(e5_dataspace_id);
    return E5_SUCCESS;
}

estatus_t
e5_write_attr_int(
    hid_t e5_group_id, const char* name, int value)
{
    e5_attr_int attr[] =
    {
        { name, value },
        { 0, 0 }
    };

    return e5_write_attr_list_int(e5_group_id, attr);
}

/* ============================================================================================== */

estatus_t
e5_read_attr_list_str(
    hid_t e5_group_id, e5_attr_str* e5_attr_list)
{
    int i;
    estatus_t status = E5_SUCCESS;

    hid_t e5_attribute_id;
    hid_t e5_dataspace_id = H5Screate(H5S_SCALAR);
    hid_t e5_string_type = H5Tcopy(H5T_C_S1);
    H5Tset_size(e5_string_type, E5_MAX_ATTR_STRING_LENGTH);

    for(i = 0; e5_attr_list && e5_attr_list[i].name != 0; i++)
    {
        e5_attr_str *attr = &e5_attr_list[i];
        if(attr->name == 0 || strlen(attr->name) < 1)
            continue;

        if(H5Aexists(e5_group_id, attr->name) <= 0)
        {
            status = E5_INVALID_ATTRIBUTE;
            e5_error(e5_group_id, status, "Specified attribute '%s' does not exist\n", attr->name);
            continue;
        }

        e5_attribute_id = H5Aopen(e5_group_id, attr->name, H5P_DEFAULT);
        H5Aread(e5_attribute_id, e5_string_type, &(attr->value));
        H5Aclose(e5_attribute_id);

        e5_info(e5_group_id, "Read attribute [type='str', name='%s', value='%s']\n",  attr->name, attr->value);
    }
    H5Sclose(e5_dataspace_id);
    return status;
}

estatus_t
e5_read_attr_str(
    hid_t e5_group_id, const char* name, char* value)
{
    e5_attr_str attr[] =
    {
        { name, {0} },
        { 0 }
    };

    estatus_t status = e5_read_attr_list_str(e5_group_id, attr);
    if(value)
        strncpy(value, attr[0].value, E5_MAX_ATTR_STRING_LENGTH);

    return status;

}

estatus_t
e5_write_attr_list_str(
    hid_t e5_group_id, e5_attr_str* e5_attr_list)
{
    int i;
    hid_t e5_attribute_id;
    hid_t e5_dataspace_id = H5Screate(H5S_SCALAR);
    hid_t e5_string_type = H5Tcopy(H5T_C_S1);
    H5Tset_size(e5_string_type, E5_MAX_ATTR_STRING_LENGTH);
    for(i = 0; e5_attr_list && e5_attr_list[i].name != 0; i++)
    {
        e5_attr_str *attr = &e5_attr_list[i];
        if(attr->name == 0 || strlen(attr->name) < 1)
            continue;

        if(attr->value == 0 || strlen(attr->value) < 1)
            continue;

        if(H5Aexists(e5_group_id, attr->name) <= 0)
            e5_attribute_id = H5Acreate(e5_group_id, attr->name, e5_string_type, e5_dataspace_id, H5P_DEFAULT);
        else
            e5_attribute_id = H5Aopen(e5_group_id, attr->name, H5P_DEFAULT);

        e5_info(e5_group_id, "Adding attribute [type='string', name='%s', value='%s']\n",  attr->name, attr->value);

        H5Awrite(e5_attribute_id, e5_string_type, &(attr->value));
        H5Aclose(e5_attribute_id);
    }
    H5Sclose(e5_dataspace_id);
    return E5_SUCCESS;
}

estatus_t
e5_write_attr_str(
    hid_t e5_group_id, const char* name, char* value)
{
    e5_attr_str attr[] =
    {
        { name, {0} },
        { 0 }
    };

    strncpy(attr[0].value, value, E5_MAX_ATTR_STRING_LENGTH);
    return e5_write_attr_list_str(e5_group_id, attr);
}

/* ============================================================================================== */

estatus_t
e5_read_attr_list_float(
    hid_t e5_group_id, e5_attr_float* e5_attr_list)
{
    int i;
    hid_t e5_attribute_id;
    estatus_t status = E5_SUCCESS;

    for(i = 0; e5_attr_list && e5_attr_list[i].name != 0; i++)
    {
        e5_attr_float *attr = &e5_attr_list[i];
        if(attr->name == 0 || strlen(attr->name) < 1)
            continue;

        if(H5Aexists(e5_group_id, attr->name) <= 0)
        {
            status = E5_INVALID_ATTRIBUTE;
            e5_error(e5_group_id, status, "Specified attribute '%s' does not exist\n", attr->name);
            continue;
        }

        e5_attribute_id = H5Aopen(e5_group_id, attr->name, H5P_DEFAULT);
        H5Aread(e5_attribute_id, H5T_NATIVE_FLOAT, &(attr->value));
        H5Aclose(e5_attribute_id);

        e5_info(e5_group_id, "Read attribute [type='float', name='%s', value='%f']\n",  attr->name, attr->value);
    }
    return status;
}

estatus_t
e5_read_attr_float(
    hid_t e5_group_id, const char* name, float* value)
{
    e5_attr_float attr[] =
    {
        { name, 0 },
        { 0 }
    };

    estatus_t status = e5_read_attr_list_float(e5_group_id, attr);
    if(value)
        *value = attr[0].value;
    return status;

}

estatus_t
e5_write_attr_list_float(
    hid_t e5_group_id, e5_attr_float* e5_attr_list)
{
    int i;
    hid_t e5_attribute_id;
    hid_t e5_dataspace_id = H5Screate(H5S_SCALAR);
    for(i = 0; e5_attr_list && e5_attr_list[i].name != 0; i++)
    {
        e5_attr_float *attr = &e5_attr_list[i];
        if(attr->name == 0 || strlen(attr->name) < 1)
            continue;

        if(H5Aexists(e5_group_id, attr->name) <= 0)
            e5_attribute_id = H5Acreate(e5_group_id, attr->name, H5T_IEEE_F32LE, e5_dataspace_id, H5P_DEFAULT);
        else
            e5_attribute_id = H5Aopen(e5_group_id, attr->name, H5P_DEFAULT);

        e5_info(e5_group_id, "Adding attribute [type='float', name='%s', value='%f']\n",  attr->name, attr->value);

        H5Awrite(e5_attribute_id, H5T_NATIVE_FLOAT, &(attr->value));
        H5Aclose(e5_attribute_id);
    }
    H5Sclose(e5_dataspace_id);
    return E5_SUCCESS;
}

estatus_t
e5_write_attr_float(
    hid_t e5_group_id, const char* name, float value)
{
    e5_attr_float attr[] =
    {
        { name, value },
        { 0, 0 }
    };

    return e5_write_attr_list_float(e5_group_id, attr);
}

/* ============================================================================================== */

estatus_t
e5_read_attr_list_double(
    hid_t e5_group_id, e5_attr_double* e5_attr_list)
{
    int i;
    hid_t e5_attribute_id;
    estatus_t status = E5_SUCCESS;

    for(i = 0; e5_attr_list && e5_attr_list[i].name != 0; i++)
    {
        e5_attr_double *attr = &e5_attr_list[i];
        if(attr->name == 0 || strlen(attr->name) < 1)
            continue;

        if(H5Aexists(e5_group_id, attr->name) <= 0)
        {
            status = E5_INVALID_ATTRIBUTE;
            e5_error(e5_group_id, status, "Specified attribute '%s' does not exist\n", attr->name);
            continue;
        }


        e5_attribute_id = H5Aopen(e5_group_id, attr->name, H5P_DEFAULT);
        H5Aread(e5_attribute_id, H5T_NATIVE_DOUBLE, &(attr->value));
        H5Aclose(e5_attribute_id);

        e5_info(e5_group_id, "Read attribute [type='double', name='%s', value='%f']\n",  attr->name, attr->value);
    }
    return status;
}

estatus_t
e5_read_attr_double(
    hid_t e5_group_id, const char* name, double* value)
{
    e5_attr_double attr[] =
    {
        { name, 0 },
        { 0, 0 }
    };

    estatus_t status = e5_read_attr_list_double(e5_group_id, attr);

    if(value)
        *value = attr[0].value;

    return status;
}

int
e5_write_attr_list_double(
    hid_t e5_group_id, e5_attr_double* e5_attr_list)
{
    int i;
    hid_t e5_attribute_id;
    hid_t e5_dataspace_id = H5Screate(H5S_SCALAR);
    for(i = 0; e5_attr_list && e5_attr_list[i].name != 0; i++)
    {
        e5_attr_double *attr = &e5_attr_list[i];
        if(attr->name == 0 || strlen(attr->name) < 1)
            continue;

        if(H5Aexists(e5_group_id, attr->name) <= 0)
            e5_attribute_id = H5Acreate(e5_group_id, attr->name, H5T_IEEE_F64LE, e5_dataspace_id, H5P_DEFAULT);
        else
            e5_attribute_id = H5Aopen(e5_group_id, attr->name, H5P_DEFAULT);

        e5_info(e5_group_id, "Adding attribute [type='double', name='%s', value='%f']\n",  attr->name, attr->value);

        H5Awrite(e5_attribute_id, H5T_NATIVE_DOUBLE, &(attr->value));
        H5Aclose(e5_attribute_id);
    }
    H5Sclose(e5_dataspace_id);
    return E5_SUCCESS;
}

estatus_t
e5_write_attr_double(
    hid_t e5_group_id, const char* name, double value)
{
    e5_attr_double attr[] =
    {
        { name, value },
        { 0, 0 }
    };

    return e5_write_attr_list_double(e5_group_id, attr);
}

/* ============================================================================================== */

estatus_t
e5_set_notify_func(eid_t e5_object_id,
    e5_notify_func callback, int min_notify_level, int max_notify_level)
{
    int min_level = min_notify_level < max_notify_level ? min_notify_level : max_notify_level;
    int max_level = max_notify_level > min_notify_level ? max_notify_level : min_notify_level;

    E5_GLOBAL_NOTIFY_FUNC = callback;
    E5_GLOBAL_NOTIFY_MIN_LEVEL = min_level;
    E5_GLOBAL_NOTIFY_MAX_LEVEL = max_level;
    return E5_SUCCESS;
}

eid_t
e5_create_file(
    const char* filename)
{
    eid_t e5_file_id;
    eid_t e5_group_id;
    estatus_t status = E5_SUCCESS;

    e5_info(E5_DEFAULT, "Creating E5 file '%s'\n", filename);

    e5_file_id = H5Fcreate(filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
    if(e5_file_id < 0)
    {
        status = E5_WRITE_FAILURE;
        e5_error(e5_file_id, status, "Failed to create '%s' E5 data file\n", filename);
        return (eid_t)e5_file_id;
    }

    e5_group_id = e5_create_group(e5_file_id, E5_EMISSIVITY_GROUP_NAME_ABS);
    e5_close_group(e5_group_id);

    return (eid_t)e5_file_id;
}

estatus_t
e5_merge_f5_file(
    eid_t e5_file_id, const char* f5_filename)
{
    int i;
    hid_t f5_file_id;
    estatus_t status;

    typedef struct
    {
        const char* name;
        f5_scalar_list_type list_type;
    } f5_scalar_list_dataset_def;

    if(!f5_filename || strlen(f5_filename) < 1)
    {
        status = E5_INVALID_FILE;
        e5_error(e5_file_id, status, "Failed to open F5 flash file '%s'!", f5_filename);
    }

    e5_info(e5_file_id, "Merging scalar lists from F5 file '%s'\n", f5_filename);

    f5_scalar_list_dataset_def f5_scalar_datasets[] =
    {
        { "integer scalars",               F5_SCALAR_LIST_INTEGER },
        { "integer runtime parameters",    F5_SCALAR_LIST_INTEGER },
        { "string scalars",                F5_SCALAR_LIST_STRING },
        { "string runtime parameters",     F5_SCALAR_LIST_STRING },
        { "real scalars",                  F5_SCALAR_LIST_REAL },
        { "real runtime parameters",       F5_SCALAR_LIST_REAL },
        { "logical scalars",               F5_SCALAR_LIST_LOGICAL },
        { "logical runtime parameters",    F5_SCALAR_LIST_LOGICAL },
        { 0 }
    };

    f5_file_id = H5Fopen(f5_filename, H5P_DEFAULT, H5P_DEFAULT);
    for(i = 0; f5_scalar_datasets[i].name != 0; i++)
    {
        f5_scalar_list_dataset_def *def = &f5_scalar_datasets[i];
        e5_merge_flash_scalars(e5_file_id, f5_file_id, def->name, def->list_type);
    }

    H5Fclose(f5_file_id);
    return E5_SUCCESS;
}

eid_t
e5_open_file(
    const char* e5_filename)
{
    eid_t status;
    hid_t e5_file_id = H5Fopen(e5_filename, H5F_ACC_RDWR, H5P_DEFAULT);

    if(e5_file_id < 0)
    {
        status = E5_WRITE_FAILURE;
        e5_error(e5_file_id, status, "Failed to create '%s' E5 data file\n", e5_filename);
        return e5_file_id;
    }
    return e5_file_id;
}

estatus_t
e5_close_file(
    eid_t e5_file_id)
{
    e5_info(e5_file_id, "Closing E5 file\n", "");

    if(e5_file_id)
        H5Fclose(e5_file_id);

    return E5_SUCCESS;
}

eid_t
e5_create_group(
    eid_t e5_group_id, const char* group_name)
{
    hid_t e5_sub_group_id;
    char* e5_group_name = (char*)e5_malloc(E5_MAX_GROUP_NAME_LENGTH+1);
    e5_group_name[E5_MAX_GROUP_NAME_LENGTH] = '\0';

    snprintf(e5_group_name, E5_MAX_GROUP_NAME_LENGTH, "%s", group_name);

    e5_group_name = e5_trim(e5_group_name, E5_FALSE, E5_TRUE, E5_MAX_GROUP_NAME_LENGTH);

    e5_info(e5_group_id, "Creating group '%s'\n", group_name);

    e5_sub_group_id = H5Gcreate(e5_group_id, e5_group_name, H5P_DEFAULT);

    e5_info(e5_group_id, "Created group '%p'\n", e5_sub_group_id);

    e5_free(e5_group_name);

    return e5_sub_group_id;
}

eid_t
e5_open_group(
    eid_t e5_group_id, const char* group_name)
{
    eid_t status;
    hid_t e5_sub_group_id;

    e5_sub_group_id = H5Gopen(e5_group_id, group_name);
    if(e5_sub_group_id < 0)
    {
        status = E5_INVALID_GROUP;
        e5_error(e5_group_id, status, "Failed to open '%s' E5 group!\n", group_name);
        return e5_sub_group_id;
    }

    e5_info(e5_group_id, "Opened group '%p' -> '%s'\n", e5_group_id, group_name);
    return e5_sub_group_id;
}

estatus_t
e5_close_group(
    eid_t e5_group_id)
{
    e5_info(e5_group_id, "Closing group '%p'\n", e5_group_id);
    H5Gclose(e5_group_id);
    return E5_SUCCESS;
}

/* ============================================================================================== */

estatus_t
e5_read_grid_list(
    eid_t e5_group_id, const char* list_name, e5_grid_dataset* grid_list)
{
    int i;
    int d;
    int log_scale;
    int close_group;

    hsize_t h5_max_dim[3];
    hsize_t h5_min_dim[3];

    eid_t e5_list_group_id;
    eid_t e5_type_id;
    eid_t e5_dataset_id;
    eid_t e5_dataspace_id;
    eid_t e5_memspace_id;

    hid_t h5_status;

    estatus_t e5_status = E5_SUCCESS;

    if(list_name && strlen(list_name))
    {
        e5_list_group_id = e5_create_group(e5_group_id, list_name);
        close_group = 1;
    }
    else
    {
        e5_list_group_id = e5_group_id;
        close_group = 0;
    }

    for(i = 0; grid_list && grid_list[i].name != 0; i++)
    {
        e5_grid_dataset* grid = &grid_list[i];
        if(grid->name == 0 || strlen(grid->name) < 1)
            continue;

        e5_dataset_id = H5Dopen(e5_list_group_id, grid->name);
        if (e5_dataset_id < 0)
        {
            e5_status = E5_INVALID_DATASET;
            e5_error(e5_list_group_id, e5_status, "Failed to open grid dataset '%s'\n",  grid->name);
            return e5_status;
        }

        if(!grid->data)
        {
            e5_status = E5_INVALID_POINTER;
            e5_error(e5_dataspace_id, e5_status, "Failed to provide pointer for reading '%s' from E5 data file\n", grid->name);
            break;
        }

        e5_dataspace_id = H5Dget_space(e5_dataset_id);
        e5_type_id = H5Dget_type(e5_dataset_id);
        H5Sget_simple_extent_dims(e5_dataspace_id, h5_min_dim, h5_max_dim);

        for(d = 0; d < 3; d++)
        {
            grid->dim[d] = h5_min_dim[d] >= h5_max_dim[d] ? h5_min_dim[d] : h5_max_dim[d];
            grid->dim[d] = grid->dim[d] < 1 ? 1 : grid->dim[d];
        }

        grid->type = e5_convert_hdf_type(e5_type_id);

        switch(grid->type)
        {
        case E5_TYPE_FLOAT:
        {
            e5_info(e5_group_id, "Reading grid [type='float', name='%s', dim='%u %u %u']\n",
                    grid->name, grid->dim[0], grid->dim[1], grid->dim[2]);

            e5_memspace_id = H5Screate_simple(3, h5_min_dim, h5_max_dim);
            h5_status = H5Dread(e5_dataset_id, H5T_NATIVE_FLOAT, e5_memspace_id, e5_dataspace_id, H5P_DEFAULT, (grid->data));
            if (h5_status < 0)
            {
                e5_status = E5_READ_FAILURE;
                e5_error(e5_dataset_id, e5_status, "Failed to read '%s' from F5 data file\n", grid->name);
            }
            H5Sclose(e5_memspace_id);
            break;
        }
        case E5_TYPE_DOUBLE:
        {
            e5_info(e5_group_id, "Reading grid [type='double', name='%s', dim='%u %u %u']\n",
                    grid->name, grid->dim[0], grid->dim[1], grid->dim[2]);

            e5_memspace_id = H5Screate_simple(3, h5_min_dim, h5_max_dim);
            h5_status = H5Dread(e5_dataset_id, H5T_NATIVE_DOUBLE, e5_memspace_id, e5_dataspace_id, H5P_DEFAULT, (grid->data));
            if (h5_status < 0)
            {
                e5_status = E5_READ_FAILURE;
                e5_error(e5_dataset_id, e5_status, "Failed to read '%s' from F5 data file\n", grid->name);
            }
            H5Sclose(e5_memspace_id);
            break;
        }
        case E5_TYPE_INVALID:
        default:
        {
            e5_status = E5_INVALID_TYPE;
            e5_error(e5_dataset_id, e5_status, "Invalid type for grid '%s' data\n", grid->name);
            break;
        }
        };

        log_scale = 0;
        if(e5_is_valid_attr(e5_dataset_id, "log10"))
            e5_read_attr_int(e5_dataset_id, "log10", &log_scale);
        grid->scale = log_scale ? E5_VALUE_SCALE_LOG10 : E5_VALUE_SCALE_LINEAR;

        H5Sclose(e5_dataspace_id);
        H5Dclose(e5_dataset_id);
        H5Tclose(e5_type_id);
    }

    if(list_name)
        e5_close_group(e5_list_group_id);

    return e5_status;
}

estatus_t
e5_read_grid(
    eid_t e5_group_id, const char* e5_grid_name,
    void* data, esize_t* dim, int* type, int* scale)
{
    e5_grid_dataset grid_list[] =
    {
        { e5_grid_name, data, { 0 }, 0, 0 },
        { 0 }
    };

    estatus_t status = e5_read_grid_list(e5_group_id, NULL, grid_list);
    if(e5_success(status) == E5_TRUE)
    {
        if(dim)
        {
            dim[0] = grid_list[0].dim[0];
            dim[1] = grid_list[0].dim[1];
            dim[2] = grid_list[0].dim[2];
        }

        if(type) *type = grid_list[0].type;
        if(scale) *scale = grid_list[0].scale;
        return E5_SUCCESS;
    }
    return status;
}

estatus_t
e5_write_grid_list(
    eid_t e5_group_id, const char* list_name, e5_grid_dataset* grid_list)
{
    int i;
    int close_group;
    estatus_t status;

    eid_t e5_list_group_id;
    eid_t e5_dataset_id;
    eid_t e5_dataspace_id;

    status = E5_SUCCESS;

    if(list_name && strlen(list_name))
    {
        e5_list_group_id = e5_create_group(e5_group_id, list_name);
        close_group = 1;
    }
    else
    {
        e5_list_group_id = e5_group_id;
        close_group = 0;
    }

    for(i = 0; grid_list && grid_list[i].name != 0 && strlen(grid_list[i].name); i++)
    {
        e5_grid_dataset* grid = &grid_list[i];
        e5_dataspace_id = H5Screate_simple(3, (hsize_t*)(grid->dim), NULL);

        switch(grid->type)
        {
        case E5_TYPE_INT:
        {
            e5_info(e5_group_id, "Writing grid [type='int', name='%s', dim='%u %u %u']\n",
                    grid->name, grid->dim[0], grid->dim[1], grid->dim[2]);

            e5_dataset_id = H5Dcreate(e5_list_group_id, grid->name, H5T_STD_I32LE, e5_dataspace_id, H5P_DEFAULT);
            status = H5Dwrite(e5_dataset_id, H5T_NATIVE_INT, H5S_ALL, e5_dataspace_id, H5P_DEFAULT, grid->data);
            if(status < 0)
            {
                status = E5_WRITE_FAILURE;
                e5_error(e5_dataset_id, status, "Failed to write '%s' E5 grid to file\n", grid->name);
            }
            break;
        }
        case E5_TYPE_FLOAT:
        {
            e5_info(e5_group_id, "Writing grid [type='float', name='%s', dim='%u %u %u']\n",
                    grid->name, grid->dim[0], grid->dim[1], grid->dim[2]);

            e5_dataset_id = H5Dcreate(e5_list_group_id, grid->name, H5T_IEEE_F32LE, e5_dataspace_id, H5P_DEFAULT);
            status = H5Dwrite(e5_dataset_id, H5T_NATIVE_FLOAT, H5S_ALL, e5_dataspace_id, H5P_DEFAULT, grid->data);
            if(status < 0)
            {
                status = E5_WRITE_FAILURE;
                e5_error(e5_dataset_id, status, "Failed to write '%s' E5 grid to file\n", grid->name);
            }
            break;
        }
        case E5_TYPE_DOUBLE:
        {
            e5_info(e5_group_id, "Writing grid [type='float', name='%s', dim='%u %u %u']\n",
                    grid->name, grid->dim[0], grid->dim[1], grid->dim[2]);

            e5_dataset_id = H5Dcreate(e5_list_group_id, grid->name, H5T_IEEE_F64LE, e5_dataspace_id, H5P_DEFAULT);
            status = H5Dwrite(e5_dataset_id, H5T_NATIVE_DOUBLE, H5S_ALL, e5_dataspace_id, H5P_DEFAULT, grid->data);
            if(status < 0)
            {
                status = E5_WRITE_FAILURE;
                e5_error(e5_dataset_id, status, "Failed to write '%s' E5 grid to file\n", grid->name);
            }
            break;
        }
        case E5_TYPE_INVALID:
        default:
        {
            status = E5_INVALID_TYPE;
            e5_error(e5_dataset_id, status, "Invalid type for grid '%s' data\n", grid->name);
        }
        break;
        };

        if(grid->scale == E5_VALUE_SCALE_LOG10)
            e5_write_attr_int(e5_dataset_id, "log10", 1);

        H5Sclose(e5_dataspace_id);
        H5Dclose(e5_dataset_id);
    }

    if(close_group)
        e5_close_group(e5_list_group_id);

    return E5_SUCCESS;
}

estatus_t
e5_write_grid(
    eid_t e5_group_id, const char* e5_grid_name,
    void* data, esize_t* dim, int type, int value_scale)
{
    e5_grid_dataset grid_list[] =
    {
        { e5_grid_name, data, { dim[0], dim[1], dim[2] }, type, value_scale },
        { 0, 0, {0,0,0}, E5_TYPE_INVALID, E5_VALUE_SCALE_INVALID }
    };

    return e5_write_grid_list(e5_group_id, NULL, grid_list);

}

estatus_t
e5_read_data_info_list(
    eid_t e5_group_id, const char* list_name, e5_data_info* info_list)
{
    int i;
    int d;
    int log_scale;
    int close_group;
    estatus_t status;

    hsize_t h5_min_dim[3];
    hsize_t h5_max_dim[3];

    eid_t e5_list_group_id;
    eid_t e5_type_id;
    eid_t e5_dataset_id;
    eid_t e5_dataspace_id;

    status = E5_SUCCESS;

    if(list_name && strlen(list_name))
    {
        e5_list_group_id = e5_create_group(e5_group_id, list_name);
        close_group = 1;
    }
    else
    {
        e5_list_group_id = e5_group_id;
        close_group = 0;
    }

    for(i = 0; info_list && info_list[i].name != 0; i++)
    {
        e5_data_info* info = &info_list[i];

        e5_dataset_id = H5Dopen(e5_list_group_id, info->name);
        if (e5_dataset_id < 0)
        {
            status = E5_INVALID_DATASET;
            e5_error(e5_list_group_id, status, "Failed to open info for dataset '%s'\n",  info->name);
            return status;
        }

        e5_dataspace_id = H5Dget_space(e5_dataset_id);
        e5_type_id = H5Dget_type(e5_dataset_id);
        H5Sget_simple_extent_dims(e5_dataspace_id, h5_min_dim, h5_max_dim);

        info->type = e5_convert_hdf_type(e5_type_id);

        for(d = 0; d < 3; d++)
        {
            info->dim[d] = h5_min_dim[d] >= h5_max_dim[d] ? h5_min_dim[d] : h5_max_dim[d];
            info->dim[d] = info->dim[d] < 1 ? 1 : info->dim[d];
        }

        log_scale = 0;
        if(e5_is_valid_attr(e5_group_id, "log10"))
            e5_read_attr_int(e5_dataset_id, "log10", &log_scale);
        info->scale = log_scale ? E5_VALUE_SCALE_LOG10 : E5_VALUE_SCALE_LINEAR;

        e5_info(e5_group_id, "Read data info [type='%s', name='%s', dim='%u %u %u']\n",
                e5_typename(info->type), info->name, info->dim[0], info->dim[1], info->dim[2]);

        H5Sclose(e5_dataspace_id);
        H5Dclose(e5_dataset_id);
        H5Tclose(e5_type_id);
    }

    if(close_group)
        e5_close_group(e5_list_group_id);

    return E5_SUCCESS;
}

estatus_t
e5_read_data_info(
    eid_t e5_group_id, const char* data_name,
    esize_t* dim, int* type, int* scale)
{
    e5_data_info info_list[] =
    {
        { data_name, { 0 }, 0, 0 },
        { 0 }
    };

    eid_t status = e5_read_data_info_list(e5_group_id, NULL, info_list);
    if(e5_success(status) == E5_TRUE)
    {
        if(dim)
        {
            dim[0] = info_list[0].dim[0];
            dim[1] = info_list[0].dim[1];
            dim[2] = info_list[0].dim[2];
        }

        if(type) *type = info_list[0].type;
        if(scale) *scale = info_list[0].scale;
        return E5_SUCCESS;
    }
    return status;
}

/* ============================================================================================== */

#endif /* VD_USE_FLASH */

