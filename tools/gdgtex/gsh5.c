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

#include "gsh5.h"
#include "hdf5.h"

/* ============================================================================================== */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stddef.h>
#include <string.h>

/* ============================================================================================== */

static vd_status_t
gsh5_notify(vd_hid_t gsh5_object_id, int level, vd_hid_t status, const char* format, ...);

#define VD_GSH5_NOTIFY_WHERESTR              "[file %s, line %d]: "
#define VD_GSH5_NOTIFY_WHEREARG              __FILE__, __LINE__

#define VD_GSH5_NOTIFY_PREFIX_TEST			"[test]"
#define VD_GSH5_NOTIFY_PREFIX_DEBUG			"[debug]"
#define VD_GSH5_NOTIFY_PREFIX_INFO			"[info]"
#define VD_GSH5_NOTIFY_PREFIX_WARNING		"[warning]"
#define VD_GSH5_NOTIFY_PREFIX_CRITICAL		"[critical]"
#define VD_GSH5_NOTIFY_PREFIX_ERROR			"[error]"
#define VD_GSH5_DEFAULT_NOTIFY_HANDLERS      (32)

#define VD_GSH5_NOTIFY(_eid, _level, _status, ... ) \
    gsh5_notify( (_eid), (_level), (_status), __VA_ARGS__ )

#define gsh5_debug(_eid, _fmt, ...) \
    VD_GSH5_NOTIFY(_eid, VD_GSH5_NOTIFY_LEVEL_DEBUG, 0, VD_GSH5_NOTIFY_WHERESTR _fmt, VD_GSH5_NOTIFY_WHEREARG, __VA_ARGS__)

#define gsh5_info(_eid, _fmt, ...)  \
    VD_GSH5_NOTIFY(_eid, VD_GSH5_NOTIFY_LEVEL_INFO, 0, _fmt, __VA_ARGS__)

#define gsh5_error(_eid, _status, _fmt, ...) \
    VD_GSH5_NOTIFY(_eid, VD_GSH5_NOTIFY_LEVEL_ERROR, _status, VD_GSH5_NOTIFY_WHERESTR _fmt, VD_GSH5_NOTIFY_WHEREARG, __VA_ARGS__)

#define gsh5_critical(_eid, _status, _fmt, ...) \
    VD_GSH5_NOTIFY(_eid, VD_GSH5_NOTIFY_LEVEL_CRITICAL, _status, VD_GSH5_NOTIFY_WHERESTR _fmt, VD_GSH5_NOTIFY_WHEREARG, __VA_ARGS__)

/* ============================================================================================== */

static gsh5_notify_func VD_GSH5_GLOBAL_NOTIFY_FUNC = 0;
static int VD_GSH5_GLOBAL_NOTIFY_MIN_LEVEL = 0;
static int VD_GSH5_GLOBAL_NOTIFY_MAX_LEVEL = 10;

/* ============================================================================================== */

typedef struct _vd_hid_t
{
    vd_hid_t	        object;
    gsh5_notify_func    notify;
} vd_internal_id_t;

static vd_bool_t
is_whitespace(char c)
{
    switch (c)
    {
    case ' ':
    case '\n':
    case '\t':
    case '\f':
    case '\r':
        return VD_GSH5_TRUE;
        break;
    default:
        return VD_GSH5_FALSE;
        break;
    }
    return VD_GSH5_FALSE;
}

static void*
gsh5_malloc(size_t bytes)
{
    void* ptr = malloc(bytes);
#ifdef VD_GSH5_DEBUG
    VD_GSH5_EXIT_ON_ERROR((ptr == NULL) ? (-1) : (VD_GSH5_SUCCESS));
#endif
    return ptr;
}

static void
gsh5_free(void* ptr)
{
#ifdef VD_GSH5_DEBUG
    VD_GSH5_EXIT_ON_ERROR((ptr == NULL) ? (-1) : (VD_GSH5_SUCCESS));
#endif
    free(ptr);
}

static char*
gsh5_trim(
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
gsh5_success(vd_status_t s)
{
    return ((s == VD_GSH5_SUCCESS) ? VD_GSH5_TRUE : VD_GSH5_FALSE);
}

const char*
gsh5_error_string(
    vd_status_t status)
{
    int value = (int)status;
    switch(value)
    {
        case VD_GSH5_SUCCESS:
            return "\0";
        case VD_GSH5_READ_FAILURE:
            return "read failure";
        case VD_GSH5_WRITE_FAILURE:
            return "write failure";
        case VD_GSH5_OUT_OF_MEMORY:
            return "out-of-memory";
        case VD_GSH5_INVALID_POINTER:
            return "invalid pointer";
        case VD_GSH5_INVALID_FILE:
            return "invalid file";
        case VD_GSH5_INVALID_GROUP:
            return "invalid group";
        case VD_GSH5_INVALID_ATTRIBUTE:
            return "invalid attribute";
        case VD_GSH5_INVALID_DATASET:
            return "invalid dataset";
        case VD_GSH5_INVALID_TYPE:
            return "invalid type";
        case VD_GSH5_UNKNOWN_FAILURE:
        default:
            return "unknown";
    }
    return "unknown";
}

vd_size_t
gsh5_sizeof(
    int type)
{
    switch(type)
    {
    case VD_GSH5_TYPE_INT:
        return sizeof(int);
    case VD_GSH5_TYPE_FLOAT:
        return sizeof(float);
    case VD_GSH5_TYPE_DOUBLE:
        return sizeof(double);
    case VD_GSH5_TYPE_FIXED_SIZE_STR:
        return sizeof(char) * VD_GSH5_MAX_ATTR_STRING_LENGTH;
    default:
        return 0;
    };

    return 0;
}

const char*
gsh5_typename(
    int type)
{
    switch(type)
    {
    case VD_GSH5_TYPE_INT:
        return "int";
    case VD_GSH5_TYPE_FLOAT:
        return "float";
    case VD_GSH5_TYPE_DOUBLE:
        return "double";
    case VD_GSH5_TYPE_FIXED_SIZE_STR:
        return "str";
    default:
        return 0;
    };

    return 0;
}

static vd_bool_t
gsh5_is_valid_type(
    int type)
{
    switch(type)
    {
    case VD_GSH5_TYPE_INT:
        return VD_GSH5_TRUE;
    case VD_GSH5_TYPE_FLOAT:
        return VD_GSH5_TRUE;
    case VD_GSH5_TYPE_DOUBLE:
        return VD_GSH5_TRUE;
    case VD_GSH5_TYPE_FIXED_SIZE_STR:
        return VD_GSH5_TRUE;
    case VD_GSH5_TYPE_INVALID:
    case VD_GSH5_TYPE_UNKNOWN:
    default:
        return VD_GSH5_FALSE;
    };

    return VD_GSH5_FALSE;
}

static hid_t
gsh5_get_native_h5_type(hid_t type)
{
    hid_t p_type;
    H5T_class_t type_class;

    type_class = H5Tget_class(type);
    if(type_class == H5T_BITFIELD)
        p_type=H5Tcopy(type);
    else
        p_type = H5Tget_native_type(type,H5T_DIR_DEFAULT);

    return(p_type);
}

#if 0
static char*
gsh5_get_group_name(const char* name)
{
    int j;
    char *group_name;

    j = (int)strlen(name) - 1;
    group_name = (char *)gsh5_malloc((size_t)j + 2);

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
gsh5_basename(const char* name)
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

static vd_status_t
gsh5_notify_handler(
    vd_hid_t gsh5_object_id,
    int level,
    vd_hid_t status,
    const char* format,
    va_list arg_list)
{
    static char buffer[4096] = {0};
    const char *prefix = 0;
    gsh5_notify_info_t info;

    if(VD_GSH5_GLOBAL_NOTIFY_FUNC == 0)
        return VD_GSH5_SUCCESS;

    if(level < VD_GSH5_GLOBAL_NOTIFY_MIN_LEVEL)
        return VD_GSH5_SUCCESS;

    if(level > VD_GSH5_GLOBAL_NOTIFY_MAX_LEVEL)
        return VD_GSH5_SUCCESS;

    switch(level)
    {
    case VD_GSH5_NOTIFY_LEVEL_DEBUG:
    {
        prefix = VD_GSH5_NOTIFY_PREFIX_DEBUG;
        break;
    }
    case VD_GSH5_NOTIFY_LEVEL_TEST:
    {
        prefix = VD_GSH5_NOTIFY_PREFIX_TEST;
        break;
    }
    case VD_GSH5_NOTIFY_LEVEL_INFO:
    {
        prefix = VD_GSH5_NOTIFY_PREFIX_INFO;
        break;
    }
    case VD_GSH5_NOTIFY_LEVEL_WARNING:
    {
        prefix = VD_GSH5_NOTIFY_PREFIX_WARNING;
        break;
    }
    case VD_GSH5_NOTIFY_LEVEL_CRITICAL:
    {
        prefix = VD_GSH5_NOTIFY_PREFIX_CRITICAL;
        break;
    }
    case VD_GSH5_NOTIFY_LEVEL_ERROR:
    {
        prefix = VD_GSH5_NOTIFY_PREFIX_ERROR;
        break;
    }
    default:
    {
        prefix = VD_GSH5_NOTIFY_PREFIX_INFO;
        break;
    }
    };

    (void)vsnprintf(buffer, sizeof(buffer), format, arg_list);

    info.level = level;
    info.status = status;
    info.object = gsh5_object_id;

    VD_GSH5_GLOBAL_NOTIFY_FUNC(buffer, &info);

    return VD_GSH5_SUCCESS;
}

static vd_status_t
gsh5_notify(
    vd_hid_t gsh5_object_id, int level, vd_hid_t status, const char* format, ...)
{
    va_list arg_list;
    va_start(arg_list, format);
    status = gsh5_notify_handler(gsh5_object_id, level, status, format, arg_list);
    va_end(arg_list);
    return status;
}

/* ============================================================================================== */

static hid_t
gsh5_convert_type_to_hdf(int type)
{
    switch( type )
    {
    case VD_GSH5_TYPE_INT:
        return H5T_STD_I32LE;
    case VD_GSH5_TYPE_FLOAT:
        return H5T_IEEE_F32LE;
    case VD_GSH5_TYPE_DOUBLE:
        return H5T_IEEE_F64LE;
    case VD_GSH5_TYPE_FIXED_SIZE_STR:
        return H5T_C_S1;
    case VD_GSH5_TYPE_INVALID:
    case VD_GSH5_TYPE_UNKNOWN:
    default:
        return 0;
    }
    return 0;
}

static int
gsh5_convert_hdf_type(hid_t gsh5_type_id)
{
    if(H5Tequal(gsh5_type_id, H5T_STD_I32LE))
        return VD_GSH5_TYPE_INT;
    else if(H5Tequal(gsh5_type_id, H5T_NATIVE_INT))
        return VD_GSH5_TYPE_INT;
    else if(H5Tequal(gsh5_type_id, H5T_IEEE_F32LE))
        return VD_GSH5_TYPE_FLOAT;
    else if(H5Tequal(gsh5_type_id, H5T_NATIVE_FLOAT))
        return VD_GSH5_TYPE_FLOAT;
    else if(H5Tequal(gsh5_type_id, H5T_IEEE_F64LE))
        return VD_GSH5_TYPE_DOUBLE;
    else if(H5Tequal(gsh5_type_id, H5T_NATIVE_DOUBLE))
        return VD_GSH5_TYPE_DOUBLE;
    else if(H5Tequal(gsh5_type_id, H5T_C_S1 ))
        return VD_GSH5_TYPE_FIXED_SIZE_STR;
    else
        return VD_GSH5_TYPE_INVALID;

    return VD_GSH5_TYPE_INVALID;
}

static hid_t
gsh5_convert_format_to_hdf(int type)
{
    switch( type )
    {
    case VD_GSH5_TYPE_INT:
        return H5T_NATIVE_INT;
    case VD_GSH5_TYPE_FLOAT:
        return H5T_NATIVE_FLOAT;
    case VD_GSH5_TYPE_DOUBLE:
        return H5T_NATIVE_DOUBLE;
    case VD_GSH5_TYPE_FIXED_SIZE_STR:
        return H5T_C_S1;
    case VD_GSH5_TYPE_INVALID:
    case VD_GSH5_TYPE_UNKNOWN:
    default:
        return 0;
    }
    return 0;

}

/* ============================================================================================== */

vd_status_t
gsh5_read_attr_list(
    hid_t gsh5_group_id, gsh5_attr* gsh5_attr_list)
{
    int i;
    vd_status_t status = VD_GSH5_SUCCESS;

    hid_t h5_type;
    hid_t h5_native_type;
    hid_t h5_space;

    hid_t gsh5_attribute_id;

    for(i = 0; gsh5_attr_list && gsh5_attr_list[i].name != 0; i++)
    {
        gsh5_attr *attr = &gsh5_attr_list[i];
        if(attr->name == 0 || strlen(attr->name) < 1)
            continue;

        gsh5_info(gsh5_group_id, "Reading attribute [name='%s']\n", attr->name);

        if(H5Aexists(gsh5_group_id, attr->name) <= 0)
        {
            status = VD_GSH5_INVALID_ATTRIBUTE;
            gsh5_error(gsh5_group_id, status, "Invalid name requested for attribute [type='%d', name='%s', value='%p']\n",  attr->type, attr->name, attr->value);
            continue;
        }

        gsh5_attribute_id = H5Aopen(gsh5_group_id, attr->name, H5P_DEFAULT);
        h5_type = H5Aget_type(gsh5_attribute_id);
        h5_space = H5Aget_space(gsh5_attribute_id);
        h5_native_type = gsh5_get_native_h5_type(h5_type);

        attr->type = gsh5_convert_hdf_type(h5_type);
        if(gsh5_is_valid_type(attr->type) != VD_GSH5_TRUE)
        {
            status = VD_GSH5_INVALID_TYPE;
            gsh5_error(gsh5_group_id, status, "Invalid type requested for attribute [type='%d', name='%s', value='%p']\n",  attr->type, attr->name, attr->value);
            continue;
        }

        H5Aread(gsh5_attribute_id, h5_native_type, attr->value);
        H5Aclose(gsh5_attribute_id);

        gsh5_info(gsh5_group_id, "Read attribute [type='%s', name='%s', value='%p']\n",
            gsh5_typename(attr->type), attr->name, attr->value);
    }
    return status;
}

vd_status_t
gsh5_write_attr_list(
    hid_t gsh5_group_id, gsh5_attr* gsh5_attr_list)
{
    int i;
    vd_hid_t status = VD_GSH5_SUCCESS;
    hid_t gsh5_attribute_id;
    hid_t gsh5_dataspace_id = H5Screate(H5S_SCALAR);
    for(i = 0; gsh5_attr_list && gsh5_attr_list[i].name != 0; i++)
    {
        gsh5_attr *attr = &gsh5_attr_list[i];
        if(attr->name == 0 || strlen(attr->name) < 1)
            continue;

        if(gsh5_is_valid_type(gsh5_attr_list[i].type) != VD_GSH5_TRUE)
        {
            status = VD_GSH5_INVALID_TYPE;
            gsh5_error(gsh5_group_id, status, "Invalid type requested for attribute [type='%d', name='%s', value='%p']\n",  attr->type, attr->name, attr->value);
            continue;
        }

        hid_t hdf_type = gsh5_convert_type_to_hdf(gsh5_attr_list[i].type);
        hid_t hdf_format = gsh5_convert_format_to_hdf(gsh5_attr_list[i].type);

        if(H5Aexists(gsh5_group_id, attr->name) <= 0)
            gsh5_attribute_id = H5Acreate(gsh5_group_id, attr->name, hdf_type, gsh5_dataspace_id, H5P_DEFAULT);

        gsh5_info(gsh5_group_id, "Adding attribute [type='integer', name='%s', value='%d']\n",  attr->name, attr->value);
        gsh5_attribute_id = H5Aopen(gsh5_group_id, attr->name, H5P_DEFAULT);
        H5Awrite(gsh5_attribute_id, hdf_format, &(attr->value));
        H5Aclose(gsh5_attribute_id);
    }
    H5Sclose(gsh5_dataspace_id);
    return VD_GSH5_SUCCESS;
}

int
gsh5_is_valid_attr(
    vd_hid_t gsh5_group_id, const char* attr_name)
{
    if(H5Aexists(gsh5_group_id, attr_name) <= 0)
        return VD_GSH5_FALSE;
    return VD_GSH5_TRUE;
}

vd_status_t
gsh5_read_attr(
    vd_hid_t gsh5_group_id, const char* attr_name, void* value, int* type, size_t* size)
{
    gsh5_attr attr_list[] =
    {
        { attr_name, value, 0, 0,  },
        { 0 }
    };

    vd_status_t status = gsh5_read_attr_list(gsh5_group_id, attr_list);
    if(gsh5_success(status) == VD_GSH5_TRUE)
    {
        if(type) *type = attr_list[0].type;
        if(size) *size = attr_list[0].size;
        return VD_GSH5_SUCCESS;
    }
    return status;
}

/* ============================================================================================== */

vd_status_t
gsh5_read_attr_list_int(
    hid_t gsh5_group_id, gsh5_attr_int* gsh5_attr_list)
{
    int i;
    hid_t gsh5_attribute_id;
    vd_status_t status = VD_GSH5_SUCCESS;

    for(i = 0; gsh5_attr_list && gsh5_attr_list[i].name != 0; i++)
    {
        gsh5_attr_int *attr = &gsh5_attr_list[i];
        if(attr->name == 0 || strlen(attr->name) < 1)
            continue;

        gsh5_info(gsh5_group_id, "Reading attribute [type='integer', name='%s']\n",  attr->name);

        if(H5Aexists(gsh5_group_id, attr->name) <= 0)
        {
            status = VD_GSH5_READ_FAILURE;
            gsh5_error(gsh5_group_id, status, "Specified attribute '%s' does not exist\n", attr->name);
            continue;
        }

        gsh5_attribute_id = H5Aopen(gsh5_group_id, attr->name, H5P_DEFAULT);
        H5Aread(gsh5_attribute_id, H5T_NATIVE_INT, &(attr->value));
        H5Aclose(gsh5_attribute_id);

        gsh5_info(gsh5_group_id, "Read attribute [type='integer', name='%s', value='%d']\n",  attr->name, attr->value);
    }
    return status;
}

vd_status_t
gsh5_read_attr_int(
    hid_t gsh5_group_id, const char* name, int* value)
{
    gsh5_attr_int attr[] =
    {
        { name, 0 },
        { 0, 0 }
    };

    vd_status_t status = gsh5_read_attr_list_int(gsh5_group_id, attr);

    if(value)
        *value = attr[0].value;

    return status;
}

vd_status_t
gsh5_write_attr_list_int(
    hid_t gsh5_group_id, gsh5_attr_int* gsh5_attr_list)
{
    int i;
    hid_t gsh5_attribute_id;
    hid_t gsh5_dataspace_id = H5Screate(H5S_SCALAR);
    for(i = 0; gsh5_attr_list && gsh5_attr_list[i].name != 0; i++)
    {
        gsh5_attr_int *attr = &gsh5_attr_list[i];
        if(attr->name == 0 || strlen(attr->name) < 1)
            continue;

        if(H5Aexists(gsh5_group_id, attr->name) <= 0)
            gsh5_attribute_id = H5Acreate(gsh5_group_id, attr->name, H5T_STD_I32LE, gsh5_dataspace_id, H5P_DEFAULT);

        gsh5_info(gsh5_group_id, "Adding attribute [type='integer', name='%s', value='%d']\n",  attr->name, attr->value);
        gsh5_attribute_id = H5Aopen(gsh5_group_id, attr->name, H5P_DEFAULT);
        H5Awrite(gsh5_attribute_id, H5T_NATIVE_INT, &(attr->value));
        H5Aclose(gsh5_attribute_id);
    }
    H5Sclose(gsh5_dataspace_id);
    return VD_GSH5_SUCCESS;
}

vd_status_t
gsh5_write_attr_int(
    hid_t gsh5_group_id, const char* name, int value)
{
    gsh5_attr_int attr[] =
    {
        { name, value },
        { 0, 0 }
    };

    return gsh5_write_attr_list_int(gsh5_group_id, attr);
}

/* ============================================================================================== */

vd_status_t
gsh5_read_attr_list_str(
    hid_t gsh5_group_id, gsh5_attr_str* gsh5_attr_list)
{
    int i;
    vd_status_t status = VD_GSH5_SUCCESS;

    hid_t gsh5_attribute_id;
    hid_t gsh5_dataspace_id = H5Screate(H5S_SCALAR);
    hid_t gsh5_string_type = H5Tcopy(H5T_C_S1);
    H5Tset_size(gsh5_string_type, VD_GSH5_MAX_ATTR_STRING_LENGTH);

    for(i = 0; gsh5_attr_list && gsh5_attr_list[i].name != 0; i++)
    {
        gsh5_attr_str *attr = &gsh5_attr_list[i];
        if(attr->name == 0 || strlen(attr->name) < 1)
            continue;

        if(H5Aexists(gsh5_group_id, attr->name) <= 0)
        {
            status = VD_GSH5_INVALID_ATTRIBUTE;
            gsh5_error(gsh5_group_id, status, "Specified attribute '%s' does not exist\n", attr->name);
            continue;
        }

        gsh5_attribute_id = H5Aopen(gsh5_group_id, attr->name, H5P_DEFAULT);
        H5Aread(gsh5_attribute_id, gsh5_string_type, &(attr->value));
        H5Aclose(gsh5_attribute_id);

        gsh5_info(gsh5_group_id, "Read attribute [type='str', name='%s', value='%s']\n",  attr->name, attr->value);
    }
    H5Sclose(gsh5_dataspace_id);
    return status;
}

vd_status_t
gsh5_read_attr_str(
    hid_t gsh5_group_id, const char* name, char* value)
{
    gsh5_attr_str attr[] =
    {
        { name, {0} },
        { 0 }
    };

    vd_status_t status = gsh5_read_attr_list_str(gsh5_group_id, attr);
    if(value)
        strncpy(value, attr[0].value, VD_GSH5_MAX_ATTR_STRING_LENGTH);

    return status;

}

vd_status_t
gsh5_write_attr_list_str(
    hid_t gsh5_group_id, gsh5_attr_str* gsh5_attr_list)
{
    int i;
    hid_t gsh5_attribute_id;
    hid_t gsh5_dataspace_id = H5Screate(H5S_SCALAR);
    hid_t gsh5_string_type = H5Tcopy(H5T_C_S1);
    H5Tset_size(gsh5_string_type, VD_GSH5_MAX_ATTR_STRING_LENGTH);
    for(i = 0; gsh5_attr_list && gsh5_attr_list[i].name != 0; i++)
    {
        gsh5_attr_str *attr = &gsh5_attr_list[i];
        if(attr->name == 0 || strlen(attr->name) < 1)
            continue;

        if(attr->value == 0 || strlen(attr->value) < 1)
            continue;

        if(H5Aexists(gsh5_group_id, attr->name) <= 0)
            gsh5_attribute_id = H5Acreate(gsh5_group_id, attr->name, gsh5_string_type, gsh5_dataspace_id, H5P_DEFAULT);
        else
            gsh5_attribute_id = H5Aopen(gsh5_group_id, attr->name, H5P_DEFAULT);

        gsh5_info(gsh5_group_id, "Adding attribute [type='string', name='%s', value='%s']\n",  attr->name, attr->value);

        H5Awrite(gsh5_attribute_id, gsh5_string_type, &(attr->value));
        H5Aclose(gsh5_attribute_id);
    }
    H5Sclose(gsh5_dataspace_id);
    return VD_GSH5_SUCCESS;
}

vd_status_t
gsh5_write_attr_str(
    hid_t gsh5_group_id, const char* name, char* value)
{
    gsh5_attr_str attr[] =
    {
        { name, {0} },
        { 0 }
    };

    strncpy(attr[0].value, value, VD_GSH5_MAX_ATTR_STRING_LENGTH);
    return gsh5_write_attr_list_str(gsh5_group_id, attr);
}

/* ============================================================================================== */

vd_status_t
gsh5_read_attr_list_float(
    hid_t gsh5_group_id, gsh5_attr_float* gsh5_attr_list)
{
    int i;
    hid_t gsh5_attribute_id;
    vd_status_t status = VD_GSH5_SUCCESS;

    for(i = 0; gsh5_attr_list && gsh5_attr_list[i].name != 0; i++)
    {
        gsh5_attr_float *attr = &gsh5_attr_list[i];
        if(attr->name == 0 || strlen(attr->name) < 1)
            continue;

        if(H5Aexists(gsh5_group_id, attr->name) <= 0)
        {
            status = VD_GSH5_INVALID_ATTRIBUTE;
            gsh5_error(gsh5_group_id, status, "Specified attribute '%s' does not exist\n", attr->name);
            continue;
        }

        gsh5_attribute_id = H5Aopen(gsh5_group_id, attr->name, H5P_DEFAULT);
        H5Aread(gsh5_attribute_id, H5T_NATIVE_FLOAT, &(attr->value));
        H5Aclose(gsh5_attribute_id);

        gsh5_info(gsh5_group_id, "Read attribute [type='float', name='%s', value='%f']\n",  attr->name, attr->value);
    }
    return status;
}

vd_status_t
gsh5_read_attr_float(
    hid_t gsh5_group_id, const char* name, float* value)
{
    gsh5_attr_float attr[] =
    {
        { name, 0 },
        { 0 }
    };

    vd_status_t status = gsh5_read_attr_list_float(gsh5_group_id, attr);
    if(value)
        *value = attr[0].value;
    return status;

}

vd_status_t
gsh5_write_attr_list_float(
    hid_t gsh5_group_id, gsh5_attr_float* gsh5_attr_list)
{
    int i;
    hid_t gsh5_attribute_id;
    hid_t gsh5_dataspace_id = H5Screate(H5S_SCALAR);
    for(i = 0; gsh5_attr_list && gsh5_attr_list[i].name != 0; i++)
    {
        gsh5_attr_float *attr = &gsh5_attr_list[i];
        if(attr->name == 0 || strlen(attr->name) < 1)
            continue;

        if(H5Aexists(gsh5_group_id, attr->name) <= 0)
            gsh5_attribute_id = H5Acreate(gsh5_group_id, attr->name, H5T_IEEE_F32LE, gsh5_dataspace_id, H5P_DEFAULT);
        else
            gsh5_attribute_id = H5Aopen(gsh5_group_id, attr->name, H5P_DEFAULT);

        gsh5_info(gsh5_group_id, "Adding attribute [type='float', name='%s', value='%f']\n",  attr->name, attr->value);

        H5Awrite(gsh5_attribute_id, H5T_NATIVE_FLOAT, &(attr->value));
        H5Aclose(gsh5_attribute_id);
    }
    H5Sclose(gsh5_dataspace_id);
    return VD_GSH5_SUCCESS;
}

vd_status_t
gsh5_write_attr_float(
    hid_t gsh5_group_id, const char* name, float value)
{
    gsh5_attr_float attr[] =
    {
        { name, value },
        { 0, 0 }
    };

    return gsh5_write_attr_list_float(gsh5_group_id, attr);
}

/* ============================================================================================== */

vd_status_t
gsh5_read_attr_list_double(
    hid_t gsh5_group_id, gsh5_attr_double* gsh5_attr_list)
{
    int i;
    hid_t gsh5_attribute_id;
    vd_status_t status = VD_GSH5_SUCCESS;

    for(i = 0; gsh5_attr_list && gsh5_attr_list[i].name != 0; i++)
    {
        gsh5_attr_double *attr = &gsh5_attr_list[i];
        if(attr->name == 0 || strlen(attr->name) < 1)
            continue;

        if(H5Aexists(gsh5_group_id, attr->name) <= 0)
        {
            status = VD_GSH5_INVALID_ATTRIBUTE;
            gsh5_error(gsh5_group_id, status, "Specified attribute '%s' does not exist\n", attr->name);
            continue;
        }


        gsh5_attribute_id = H5Aopen(gsh5_group_id, attr->name, H5P_DEFAULT);
        H5Aread(gsh5_attribute_id, H5T_NATIVE_DOUBLE, &(attr->value));
        H5Aclose(gsh5_attribute_id);

        gsh5_info(gsh5_group_id, "Read attribute [type='double', name='%s', value='%f']\n",  attr->name, attr->value);
    }
    return status;
}

vd_status_t
gsh5_read_attr_double(
    hid_t gsh5_group_id, const char* name, double* value)
{
    gsh5_attr_double attr[] =
    {
        { name, 0 },
        { 0, 0 }
    };

    vd_status_t status = gsh5_read_attr_list_double(gsh5_group_id, attr);

    if(value)
        *value = attr[0].value;

    return status;
}

int
gsh5_write_attr_list_double(
    hid_t gsh5_group_id, gsh5_attr_double* gsh5_attr_list)
{
    int i;
    hid_t gsh5_attribute_id;
    hid_t gsh5_dataspace_id = H5Screate(H5S_SCALAR);
    for(i = 0; gsh5_attr_list && gsh5_attr_list[i].name != 0; i++)
    {
        gsh5_attr_double *attr = &gsh5_attr_list[i];
        if(attr->name == 0 || strlen(attr->name) < 1)
            continue;

        if(H5Aexists(gsh5_group_id, attr->name) <= 0)
            gsh5_attribute_id = H5Acreate(gsh5_group_id, attr->name, H5T_IEEE_F64LE, gsh5_dataspace_id, H5P_DEFAULT);
        else
            gsh5_attribute_id = H5Aopen(gsh5_group_id, attr->name, H5P_DEFAULT);

        gsh5_info(gsh5_group_id, "Adding attribute [type='double', name='%s', value='%f']\n",  attr->name, attr->value);

        H5Awrite(gsh5_attribute_id, H5T_NATIVE_DOUBLE, &(attr->value));
        H5Aclose(gsh5_attribute_id);
    }
    H5Sclose(gsh5_dataspace_id);
    return VD_GSH5_SUCCESS;
}

vd_status_t
gsh5_write_attr_double(
    hid_t gsh5_group_id, const char* name, double value)
{
    gsh5_attr_double attr[] =
    {
        { name, value },
        { 0, 0 }
    };

    return gsh5_write_attr_list_double(gsh5_group_id, attr);
}

/* ============================================================================================== */

vd_status_t
gsh5_set_notify_func(vd_hid_t gsh5_object_id,
    gsh5_notify_func callback, int min_notify_level, int max_notify_level)
{
    int min_level = min_notify_level < max_notify_level ? min_notify_level : max_notify_level;
    int max_level = max_notify_level > min_notify_level ? max_notify_level : min_notify_level;

    VD_GSH5_GLOBAL_NOTIFY_FUNC = callback;
    VD_GSH5_GLOBAL_NOTIFY_MIN_LEVEL = min_level;
    VD_GSH5_GLOBAL_NOTIFY_MAX_LEVEL = max_level;
    return VD_GSH5_SUCCESS;
}

vd_hid_t
gsh5_create_file(
    const char* filename)
{
    vd_hid_t gsh5_file_id;
    vd_hid_t gsh5_group_id;
    vd_status_t status = VD_GSH5_SUCCESS;

    gsh5_info(VD_GSH5_DEFAULT, "Creating H5 file '%s'\n", filename);

    gsh5_file_id = H5Fcreate(filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
    if(gsh5_file_id < 0)
    {
        status = VD_GSH5_WRITE_FAILURE;
        gsh5_error(gsh5_file_id, status, "Failed to create '%s' H5 data file\n", filename);
        return (vd_hid_t)gsh5_file_id;
    }

    return (vd_hid_t)gsh5_file_id;
}

vd_hid_t
gsh5_open_file(
    const char* gsh5_filename)
{
    vd_hid_t status;
    hid_t gsh5_file_id = H5Fopen(gsh5_filename, H5F_ACC_RDWR, H5P_DEFAULT);

    if(gsh5_file_id < 0)
    {
        status = VD_GSH5_WRITE_FAILURE;
        gsh5_error(gsh5_file_id, status, "Failed to create '%s' H5 data file\n", gsh5_filename);
        return gsh5_file_id;
    }
    return gsh5_file_id;
}

vd_status_t
gsh5_close_file(
    vd_hid_t gsh5_file_id)
{
    gsh5_info(gsh5_file_id, "Closing H5 file\n", "");

    if(gsh5_file_id)
        H5Fclose(gsh5_file_id);

    return VD_GSH5_SUCCESS;
}

vd_hid_t
gsh5_create_group(
    vd_hid_t gsh5_group_id, const char* group_name)
{
    hid_t gsh5_sub_group_id;
    char* gsh5_group_name = (char*)gsh5_malloc(VD_GSH5_MAX_GROUP_NAME_LENGTH+1);
    gsh5_group_name[VD_GSH5_MAX_GROUP_NAME_LENGTH] = '\0';

    snprintf(gsh5_group_name, VD_GSH5_MAX_GROUP_NAME_LENGTH, "%s", group_name);

    gsh5_group_name = gsh5_trim(gsh5_group_name, VD_GSH5_FALSE, VD_GSH5_TRUE, VD_GSH5_MAX_GROUP_NAME_LENGTH);

    gsh5_info(gsh5_group_id, "Creating group '%s'\n", group_name);

    gsh5_sub_group_id = H5Gcreate(gsh5_group_id, gsh5_group_name, H5P_DEFAULT);

    gsh5_info(gsh5_group_id, "Created group '%p'\n", gsh5_sub_group_id);

    gsh5_free(gsh5_group_name);

    return gsh5_sub_group_id;
}

vd_hid_t
gsh5_open_group(
    vd_hid_t gsh5_group_id, const char* group_name)
{
    vd_hid_t status;
    hid_t gsh5_sub_group_id;

    gsh5_sub_group_id = H5Gopen(gsh5_group_id, group_name);
    if(gsh5_sub_group_id < 0)
    {
        status = VD_GSH5_INVALID_GROUP;
        gsh5_error(gsh5_group_id, status, "Failed to open '%s' H5 group!\n", group_name);
        return gsh5_sub_group_id;
    }

    gsh5_info(gsh5_group_id, "Opened group '%p' -> '%s'\n", gsh5_group_id, group_name);
    return gsh5_sub_group_id;
}

vd_status_t
gsh5_close_group(
    vd_hid_t gsh5_group_id)
{
    gsh5_info(gsh5_group_id, "Closing group '%p'\n", gsh5_group_id);
    H5Gclose(gsh5_group_id);
    return VD_GSH5_SUCCESS;
}

/* ============================================================================================== */

vd_status_t
gsh5_read_grid_list(
    vd_hid_t gsh5_group_id, const char* list_name, gsh5_grid_dataset* grid_list)
{
    int i;
    int d;
    int log_scale;
    int close_group;

    hsize_t h5_max_dim[3];
    hsize_t h5_min_dim[3];

    vd_hid_t gsh5_list_group_id;
    vd_hid_t gsh5_type_id;
    vd_hid_t gsh5_dataset_id;
    vd_hid_t gsh5_dataspace_id;
    vd_hid_t gsh5_memspace_id;

    hid_t h5_status;

    vd_status_t gsh5_status = VD_GSH5_SUCCESS;

    if(list_name && strlen(list_name))
    {
        gsh5_list_group_id = gsh5_create_group(gsh5_group_id, list_name);
        close_group = 1;
    }
    else
    {
        gsh5_list_group_id = gsh5_group_id;
        close_group = 0;
    }

    for(i = 0; grid_list && grid_list[i].name != 0; i++)
    {
        gsh5_grid_dataset* grid = &grid_list[i];
        if(grid->name == 0 || strlen(grid->name) < 1)
            continue;

        gsh5_dataset_id = H5Dopen(gsh5_list_group_id, grid->name);
        if (gsh5_dataset_id < 0)
        {
            gsh5_status = VD_GSH5_INVALID_DATASET;
            gsh5_error(gsh5_list_group_id, gsh5_status, "Failed to open grid dataset '%s'\n",  grid->name);
            return gsh5_status;
        }

        if(!grid->data)
        {
            gsh5_status = VD_GSH5_INVALID_POINTER;
            gsh5_error(gsh5_dataspace_id, gsh5_status, "Failed to provide pointer for reading '%s' from H5 data file\n", grid->name);
            break;
        }

        gsh5_dataspace_id = H5Dget_space(gsh5_dataset_id);
        gsh5_type_id = H5Dget_type(gsh5_dataset_id);
        H5Sget_simple_extent_dims(gsh5_dataspace_id, h5_min_dim, h5_max_dim);

        for(d = 0; d < 3; d++)
        {
            grid->dim[d] = h5_min_dim[d] >= h5_max_dim[d] ? h5_min_dim[d] : h5_max_dim[d];
            grid->dim[d] = grid->dim[d] < 1 ? 1 : grid->dim[d];
        }

        grid->type = gsh5_convert_hdf_type(gsh5_type_id);

        switch(grid->type)
        {
        case VD_GSH5_TYPE_FLOAT:
        {
            gsh5_info(gsh5_group_id, "Reading grid [type='float', name='%s', dim='%u %u %u']\n",
                    grid->name, grid->dim[0], grid->dim[1], grid->dim[2]);

            gsh5_memspace_id = H5Screate_simple(3, h5_min_dim, h5_max_dim);
            h5_status = H5Dread(gsh5_dataset_id, H5T_NATIVE_FLOAT, gsh5_memspace_id, gsh5_dataspace_id, H5P_DEFAULT, (grid->data));
            if (h5_status < 0)
            {
                gsh5_status = VD_GSH5_READ_FAILURE;
                gsh5_error(gsh5_dataset_id, gsh5_status, "Failed to read '%s' from F5 data file\n", grid->name);
            }
            H5Sclose(gsh5_memspace_id);
            break;
        }
        case VD_GSH5_TYPE_DOUBLE:
        {
            gsh5_info(gsh5_group_id, "Reading grid [type='double', name='%s', dim='%u %u %u']\n",
                    grid->name, grid->dim[0], grid->dim[1], grid->dim[2]);

            gsh5_memspace_id = H5Screate_simple(3, h5_min_dim, h5_max_dim);
            h5_status = H5Dread(gsh5_dataset_id, H5T_NATIVE_DOUBLE, gsh5_memspace_id, gsh5_dataspace_id, H5P_DEFAULT, (grid->data));
            if (h5_status < 0)
            {
                gsh5_status = VD_GSH5_READ_FAILURE;
                gsh5_error(gsh5_dataset_id, gsh5_status, "Failed to read '%s' from F5 data file\n", grid->name);
            }
            H5Sclose(gsh5_memspace_id);
            break;
        }
        case VD_GSH5_TYPE_INVALID:
        default:
        {
            gsh5_status = VD_GSH5_INVALID_TYPE;
            gsh5_error(gsh5_dataset_id, gsh5_status, "Invalid type for grid '%s' data\n", grid->name);
            break;
        }
        };

        log_scale = 0;
        if(gsh5_is_valid_attr(gsh5_dataset_id, "log10"))
            gsh5_read_attr_int(gsh5_dataset_id, "log10", &log_scale);
        grid->scale = log_scale ? VD_GSH5_VALUE_SCALE_LOG10 : VD_GSH5_VALUE_SCALE_LINEAR;

        H5Sclose(gsh5_dataspace_id);
        H5Dclose(gsh5_dataset_id);
        H5Tclose(gsh5_type_id);
    }

    if(list_name)
        gsh5_close_group(gsh5_list_group_id);

    return gsh5_status;
}

vd_status_t
gsh5_read_grid(
    vd_hid_t gsh5_group_id, const char* gsh5_grid_name,
    void* data, vd_size_t* dim, int* type, int* scale)
{
    gsh5_grid_dataset grid_list[] =
    {
        { gsh5_grid_name, data, { 0 }, 0, 0 },
        { 0 }
    };

    vd_status_t status = gsh5_read_grid_list(gsh5_group_id, NULL, grid_list);
    if(gsh5_success(status) == VD_GSH5_TRUE)
    {
        if(dim)
        {
            dim[0] = grid_list[0].dim[0];
            dim[1] = grid_list[0].dim[1];
            dim[2] = grid_list[0].dim[2];
        }

        if(type) *type = grid_list[0].type;
        if(scale) *scale = grid_list[0].scale;
        return VD_GSH5_SUCCESS;
    }
    return status;
}

vd_status_t
gsh5_write_grid_list(
    vd_hid_t gsh5_group_id, const char* list_name, gsh5_grid_dataset* grid_list)
{
    int i;
    int close_group;
    vd_status_t status;

    vd_hid_t gsh5_list_group_id;
    vd_hid_t gsh5_dataset_id;
    vd_hid_t gsh5_dataspace_id;

    status = VD_GSH5_SUCCESS;

    if(list_name && strlen(list_name))
    {
        gsh5_list_group_id = gsh5_create_group(gsh5_group_id, list_name);
        close_group = 1;
    }
    else
    {
        gsh5_list_group_id = gsh5_group_id;
        close_group = 0;
    }

    for(i = 0; grid_list && grid_list[i].name != 0 && strlen(grid_list[i].name); i++)
    {
        gsh5_grid_dataset* grid = &grid_list[i];
        gsh5_dataspace_id = H5Screate_simple(3, (hsize_t*)(grid->dim), NULL);

        switch(grid->type)
        {
        case VD_GSH5_TYPE_INT:
        {
            gsh5_info(gsh5_group_id, "Writing grid [type='int', name='%s', dim='%u %u %u']\n",
                    grid->name, grid->dim[0], grid->dim[1], grid->dim[2]);

            gsh5_dataset_id = H5Dcreate(gsh5_list_group_id, grid->name, H5T_STD_I32LE, gsh5_dataspace_id, H5P_DEFAULT);
            status = H5Dwrite(gsh5_dataset_id, H5T_NATIVE_INT, H5S_ALL, gsh5_dataspace_id, H5P_DEFAULT, grid->data);
            if(status < 0)
            {
                status = VD_GSH5_WRITE_FAILURE;
                gsh5_error(gsh5_dataset_id, status, "Failed to write '%s' H5 grid to file\n", grid->name);
            }
            break;
        }
        case VD_GSH5_TYPE_FLOAT:
        {
            gsh5_info(gsh5_group_id, "Writing grid [type='float', name='%s', dim='%u %u %u']\n",
                    grid->name, grid->dim[0], grid->dim[1], grid->dim[2]);

            gsh5_dataset_id = H5Dcreate(gsh5_list_group_id, grid->name, H5T_IEEE_F32LE, gsh5_dataspace_id, H5P_DEFAULT);
            status = H5Dwrite(gsh5_dataset_id, H5T_NATIVE_FLOAT, H5S_ALL, gsh5_dataspace_id, H5P_DEFAULT, grid->data);
            if(status < 0)
            {
                status = VD_GSH5_WRITE_FAILURE;
                gsh5_error(gsh5_dataset_id, status, "Failed to write '%s' H5 grid to file\n", grid->name);
            }
            break;
        }
        case VD_GSH5_TYPE_DOUBLE:
        {
            gsh5_info(gsh5_group_id, "Writing grid [type='float', name='%s', dim='%u %u %u']\n",
                    grid->name, grid->dim[0], grid->dim[1], grid->dim[2]);

            gsh5_dataset_id = H5Dcreate(gsh5_list_group_id, grid->name, H5T_IEEE_F64LE, gsh5_dataspace_id, H5P_DEFAULT);
            status = H5Dwrite(gsh5_dataset_id, H5T_NATIVE_DOUBLE, H5S_ALL, gsh5_dataspace_id, H5P_DEFAULT, grid->data);
            if(status < 0)
            {
                status = VD_GSH5_WRITE_FAILURE;
                gsh5_error(gsh5_dataset_id, status, "Failed to write '%s' H5 grid to file\n", grid->name);
            }
            break;
        }
        case VD_GSH5_TYPE_INVALID:
        default:
        {
            status = VD_GSH5_INVALID_TYPE;
            gsh5_error(gsh5_dataset_id, status, "Invalid type for grid '%s' data\n", grid->name);
        }
        break;
        };

        if(grid->scale == VD_GSH5_VALUE_SCALE_LOG10)
            gsh5_write_attr_int(gsh5_dataset_id, "log10", 1);

        H5Sclose(gsh5_dataspace_id);
        H5Dclose(gsh5_dataset_id);
    }

    if(close_group)
        gsh5_close_group(gsh5_list_group_id);

    return VD_GSH5_SUCCESS;
}

vd_status_t
gsh5_write_grid(
    vd_hid_t gsh5_group_id, const char* gsh5_grid_name,
    void* data, vd_size_t* dim, int type, int value_scale)
{
    gsh5_grid_dataset grid_list[] =
    {
        { gsh5_grid_name, data, { dim[0], dim[1], dim[2] }, type, value_scale },
        { 0, 0, {0,0,0}, VD_GSH5_TYPE_INVALID, VD_GSH5_VALUE_SCALE_INVALID }
    };

    return gsh5_write_grid_list(gsh5_group_id, NULL, grid_list);

}

vd_status_t
gsh5_read_data_info_list(
    vd_hid_t gsh5_group_id, const char* list_name, gsh5_data_info* info_list)
{
    int i;
    int d;
    int log_scale;
    int close_group;
    vd_status_t status;

    hsize_t h5_min_dim[3];
    hsize_t h5_max_dim[3];

    vd_hid_t gsh5_list_group_id;
    vd_hid_t gsh5_type_id;
    vd_hid_t gsh5_dataset_id;
    vd_hid_t gsh5_dataspace_id;

    status = VD_GSH5_SUCCESS;

    if(list_name && strlen(list_name))
    {
        gsh5_list_group_id = gsh5_create_group(gsh5_group_id, list_name);
        close_group = 1;
    }
    else
    {
        gsh5_list_group_id = gsh5_group_id;
        close_group = 0;
    }

    for(i = 0; info_list && info_list[i].name != 0; i++)
    {
        gsh5_data_info* info = &info_list[i];

        gsh5_dataset_id = H5Dopen(gsh5_list_group_id, info->name);
        if (gsh5_dataset_id < 0)
        {
            status = VD_GSH5_INVALID_DATASET;
            gsh5_error(gsh5_list_group_id, status, "Failed to open info for dataset '%s'\n",  info->name);
            return status;
        }

        gsh5_dataspace_id = H5Dget_space(gsh5_dataset_id);
        gsh5_type_id = H5Dget_type(gsh5_dataset_id);
        H5Sget_simple_extent_dims(gsh5_dataspace_id, h5_min_dim, h5_max_dim);

        info->type = gsh5_convert_hdf_type(gsh5_type_id);

        for(d = 0; d < 3; d++)
        {
            info->dim[d] = h5_min_dim[d] >= h5_max_dim[d] ? h5_min_dim[d] : h5_max_dim[d];
            info->dim[d] = info->dim[d] < 1 ? 1 : info->dim[d];
        }

        log_scale = 0;
        if(gsh5_is_valid_attr(gsh5_group_id, "log10"))
            gsh5_read_attr_int(gsh5_dataset_id, "log10", &log_scale);
        info->scale = log_scale ? VD_GSH5_VALUE_SCALE_LOG10 : VD_GSH5_VALUE_SCALE_LINEAR;

        gsh5_info(gsh5_group_id, "Read data info [type='%s', name='%s', dim='%u %u %u']\n",
                gsh5_typename(info->type), info->name, info->dim[0], info->dim[1], info->dim[2]);

        H5Sclose(gsh5_dataspace_id);
        H5Dclose(gsh5_dataset_id);
        H5Tclose(gsh5_type_id);
    }

    if(close_group)
        gsh5_close_group(gsh5_list_group_id);

    return VD_GSH5_SUCCESS;
}

vd_status_t
gsh5_read_data_info(
    vd_hid_t gsh5_group_id, const char* data_name,
    vd_size_t* dim, int* type, int* scale)
{
    gsh5_data_info info_list[] =
    {
        { data_name, { 0 }, 0, 0 },
        { 0 }
    };

    vd_hid_t status = gsh5_read_data_info_list(gsh5_group_id, NULL, info_list);
    if(gsh5_success(status) == VD_GSH5_TRUE)
    {
        if(dim)
        {
            dim[0] = info_list[0].dim[0];
            dim[1] = info_list[0].dim[1];
            dim[2] = info_list[0].dim[2];
        }

        if(type) *type = info_list[0].type;
        if(scale) *scale = info_list[0].scale;
        return VD_GSH5_SUCCESS;
    }
    return status;
}

/* ============================================================================================== */

#endif /* VD_USE_FLASH */

