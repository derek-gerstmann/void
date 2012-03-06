// ============================================================================================== //
//
// License:   The Lesser GNU Public License (LGPL) v3.0.
//
// Author(s):   Derek Gerstmann. The University of Western Australia (UWA).
//        As well as the shoulders of many giants...
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
// ============================================================================================== //

#if defined(VD_USE_HDF5)

// ============================================================================================== //

#include <hdf5.h>
#include "formats/hdf.h"
#include "core/asserts.h"
#include "core/exceptions.h"
#include "constants/constants.h"

#include <string.h>
#include <math.h>

// ============================================================================================== //

VD_FORMATS_NAMESPACE_BEGIN();

// ============================================================================================== //

namespace Hdf
{

// ---------------------------------------------------------------------------------------------- //

bool
ReadAttribute(
    hid_t location, 
    const vd::string& attrib_name, 
    vd::string& value)
{
    H5T_class_t type_class;
    H5A_info_t attrib_info;
    hsize_t str_length;

    int exists = H5Aexists(location, attrib_name.c_str());
    vdGlobalAssertMsg(exists, "Couldn't find attribute %s", attrib_name.c_str());

    AttribDataAccess attrib_data(location, attrib_name.c_str(), H5P_DEFAULT);
    AttribSpaceAccess attrib_space(attrib_data);
    AttribTypeAccess attrib_type(attrib_data);

    int info = H5Aget_info(attrib_data, &attrib_info);
    vdGlobalAssertMsg(info, "Couldn't get attribute info %s", attrib_name.c_str());

    str_length = attrib_info.data_size;
    type_class = H5Tget_class(attrib_type);
    vdGlobalAssertMsg(type_class == H5T_STRING, "Bad attribute type class for %s", attrib_name.c_str());

    NativeTypeAccess native_type(attrib_type, H5T_DIR_ASCEND);
    std::vector<char> temp_str(str_length + 1);

    int read = H5Aread(attrib_data, native_type, &temp_str[0]);
    vdGlobalAssertMsg(read, "Couldn't read attribute %s", attrib_name.c_str());

    value = vd::string(&temp_str[0]);
    return true;
}

// ---------------------------------------------------------------------------------------------- //

bool
ReadAttribute(
    hid_t location, 
    const vd::string& attrib_name,
    unsigned int attrib_size, int& value)
{
    H5T_class_t type_class;
    int exists = H5Aexists(location, attrib_name.c_str());
    vdGlobalAssertMsg(exists, "Couldn't find attribute %s", attrib_name.c_str());

    AttribDataAccess attrib_data(location, attrib_name.c_str(), H5P_DEFAULT);
    AttribSpaceAccess attrib_space(attrib_data);
    AttribTypeAccess attrib_type(attrib_data);

    int extents = H5Sget_simple_extent_ndims(attrib_space);
    vdGlobalAssertMsg(extents, "Bad attribute rank for attribute %s", attrib_name.c_str());

    hsize_t dims[1];
    H5Sget_simple_extent_dims(attrib_space, dims, NULL);
    vdGlobalAssertMsg(dims[0] == attrib_size, "Invalid attribute size for attribute %s", attrib_name.c_str());

    type_class = H5Tget_class(attrib_type);
    vdGlobalAssertMsg(type_class == H5T_INTEGER, "Bad attribute type class for %s", attrib_name.c_str());

    NativeTypeAccess nativeType(attrib_type, H5T_DIR_ASCEND);
    int read = H5Aread(attrib_data, nativeType, &value);
    vdGlobalAssertMsg(read, "Couldn't read attribute %s", attrib_name.c_str());

    return true;

}

// ---------------------------------------------------------------------------------------------- //

bool
ReadAttribute(
    hid_t location, 
    const vd::string& attrib_name,
    unsigned int attrib_size, 
    float& value)
{
    H5T_class_t type_class;

    int exists = H5Aexists(location, attrib_name.c_str());
    vdGlobalAssertMsg(exists, "Couldn't find attribute %s", attrib_name.c_str());

    AttribDataAccess attrib_data(location, attrib_name.c_str(), H5P_DEFAULT);
    AttribSpaceAccess attrib_space(attrib_data);
    AttribTypeAccess attrib_type(attrib_data);

    int extents = H5Sget_simple_extent_ndims(attrib_space);
    vdGlobalAssertMsg(extents, "Bad attribute rank for attribute %s", attrib_name.c_str());

    hsize_t dims[1];
    H5Sget_simple_extent_dims(attrib_space, dims, NULL);
    vdGlobalAssertMsg(dims[0] == attrib_size, "Invalid attribute size for attribute %s", attrib_name.c_str());

    type_class = H5Tget_class(attrib_type);
    vdGlobalAssertMsg(type_class == H5T_FLOAT, "Bad attribute type class for %s", attrib_name.c_str());

    NativeTypeAccess native_type(attrib_type, H5T_DIR_ASCEND);
    int read = H5Aread(attrib_data, native_type, &value);
    vdGlobalAssertMsg(read, "Couldn't read attribute %s", attrib_name.c_str());

    return true;
}

// ---------------------------------------------------------------------------------------------- //

bool
ReadAttribute(
    hid_t location, 
    const vd::string& attrib_name,
    unsigned int attrib_size, 
    double& value)
{
    H5T_class_t type_class;

    int exists = H5Aexists(location, attrib_name.c_str());
    vdGlobalAssertMsg(exists, "Couldn't find attribute %s", attrib_name.c_str());

    AttribDataAccess attrib_data(location, attrib_name.c_str(), H5P_DEFAULT);
    AttribSpaceAccess attrib_space(attrib_data);
    AttribTypeAccess attrib_type(attrib_data);

    int extents = H5Sget_simple_extent_ndims(attrib_space);
    vdGlobalAssertMsg(extents == 1, "Bad attribute rank for attribute ", attrib_name.c_str());

    hsize_t dims[1];
    H5Sget_simple_extent_dims(attrib_space, dims, NULL);
    vdGlobalAssertMsg(dims[0] == attrib_size, "Invalid attribute size for attribute %s", attrib_name.c_str());

    type_class = H5Tget_class(attrib_type);
    vdGlobalAssertMsg(type_class == H5T_FLOAT, "Bad attribute type class for %s", attrib_name.c_str());

    NativeTypeAccess native_type(attrib_type, H5T_DIR_ASCEND);
    int read = H5Aread(attrib_data, native_type, &value);
    vdGlobalAssertMsg(read, "Couldn't read attribute %s", attrib_name.c_str());

    return true;
}

// ---------------------------------------------------------------------------------------------- //

bool
WriteAttribute(
    hid_t location, 
    const vd::string& attrib_name, 
    const vd::string& value)
{
    hid_t attrib_data = -1;
    hid_t attrib_space;
    hid_t attrib_type;

    bool success = true;

    attrib_space = H5Screate(H5S_SCALAR);

    if(attrib_space < 0)
        success = false;

    attrib_type = H5Tcopy(H5T_C_S1);
    if(attrib_type < 0)
        success = false;

    if(value.size())
    {
        if(success && H5Tset_size(attrib_type, value.size()) < 0)
        {
            success = false;
        }
    }

    if(success)
    {
        H5Tset_strpad(attrib_type, H5T_STR_NULLTERM);
        attrib_data = H5Acreate(location, attrib_name.c_str(), attrib_type, attrib_space,
                                H5P_DEFAULT, H5P_DEFAULT);
    }

    if(attrib_data < 0)
    {
        vdLogGlobalWarning("Error creating attribute: %s", attrib_name.c_str());
        success = false;
    }

    if(success && H5Awrite(attrib_data, attrib_type, value.c_str()) < 0)
    {
        vdLogGlobalWarning("Error writing attribute: %s", attrib_name.c_str());
        success = false;
    }

    H5Aclose(attrib_data);
    H5Tclose(attrib_type);
    H5Sclose(attrib_space);

    return success;

}

// ---------------------------------------------------------------------------------------------- //

bool
WriteAttribute(
    hid_t location, 
    const vd::string& attrib_name,
    unsigned int attrib_size, 
    const int& value)
{
    hid_t attrib_data;
    hid_t attrib_space;
    hsize_t dims[1];

    dims[0] = attrib_size;

    attrib_space = H5Screate(H5S_SIMPLE);
    if(attrib_space < 0)
        return false;

    if(H5Sset_extent_simple(attrib_space, 1, dims, NULL) < 0)
        return false;

    attrib_data = H5Acreate(location, attrib_name.c_str(), H5T_NATIVE_INT,
                            attrib_space, H5P_DEFAULT, H5P_DEFAULT);

    if(attrib_data < 0)
    {
        vdLogGlobalWarning("Error creating attribute: %s", attrib_name.c_str());
        H5Aclose(attrib_data);
        H5Sclose(attrib_space);
        return false;
    }

    if(H5Awrite(attrib_data, H5T_NATIVE_INT, &value) < 0)
    {
        vdLogGlobalWarning("Error writing attribute: %s", attrib_name.c_str());
        H5Aclose(attrib_data);
        H5Sclose(attrib_space);
        return false;
    }

    H5Aclose(attrib_data);
    H5Sclose(attrib_space);

    return true;
}

// ---------------------------------------------------------------------------------------------- //

bool
WriteAttribute(
    hid_t location, 
    const vd::string& attrib_name,
    unsigned int attrib_size, 
    const float& value)
{
    hid_t attrib_data;
    hid_t attrib_space;
    hsize_t dims[1];

    dims[0] = attrib_size;

    attrib_space = H5Screate(H5S_SIMPLE);
    if(attrib_space < 0)
        return false;

    if(H5Sset_extent_simple(attrib_space, 1, dims, NULL) < 0)
        return false;

    attrib_data = H5Acreate(location, attrib_name.c_str(), H5T_NATIVE_FLOAT,
                            attrib_space, H5P_DEFAULT, H5P_DEFAULT);

    if(attrib_data < 0)
    {
        vdLogGlobalWarning("Error creating attribute: %s", attrib_name.c_str());
        H5Aclose(attrib_data);
        H5Sclose(attrib_space);
        return false;
    }

    if(H5Awrite(attrib_data, H5T_NATIVE_FLOAT, &value) < 0)
    {
        vdLogGlobalWarning("Error writing attribute: %s", attrib_name.c_str());
        H5Aclose(attrib_data);
        H5Sclose(attrib_space);
        return false;
    }

    H5Aclose(attrib_data);
    H5Sclose(attrib_space);

    return true;
}

// ---------------------------------------------------------------------------------------------- //

bool
WriteAttribute(
    hid_t location, 
    const vd::string& attrib_name,
    unsigned int attrib_size, 
    const double& value)
{
    hid_t attrib_data;
    hid_t attrib_space;
    hsize_t dims[1];

    dims[0] = attrib_size;

    attrib_space = H5Screate(H5S_SIMPLE);

    if(attrib_space < 0)
        return false;

    if(H5Sset_extent_simple(attrib_space, 1, dims, NULL) < 0)
        return false;

    attrib_data = H5Acreate(location, attrib_name.c_str(), H5T_NATIVE_DOUBLE,
                            attrib_space, H5P_DEFAULT, H5P_DEFAULT);

    if(attrib_data < 0)
    {
        vdLogGlobalWarning("Error creating attribute: %s", attrib_name.c_str());
        H5Aclose(attrib_data);
        H5Sclose(attrib_space);
        return false;
    }

    if(H5Awrite(attrib_data, H5T_NATIVE_DOUBLE, &value) < 0)
    {
        vdLogGlobalWarning("Error writing attribute: %s", attrib_name.c_str());
        H5Aclose(attrib_data);
        H5Sclose(attrib_space);
        return false;
    }

    H5Aclose(attrib_data);
    H5Sclose(attrib_space);

    return true;
}

// ---------------------------------------------------------------------------------------------- //

bool IsSupported(
    Hdf::Feature::Value value)
{
    htri_t avail = -1;
    switch(value)
    {
        case Hdf::Feature::ZLib: 
        {
            avail = H5Zfilter_avail(H5Z_FILTER_DEFLATE);
            if(avail < 0) return false;

            unsigned int filter_info;
            herr_t status = H5Zget_filter_info(H5Z_FILTER_DEFLATE, &filter_info);
            if(status < 0) return false;

            if( !(filter_info & H5Z_FILTER_CONFIG_ENCODE_ENABLED) ||
                !(filter_info & H5Z_FILTER_CONFIG_DECODE_ENABLED))
            {
                return false;
            }
            return true;
        }
        default: { avail = -1; break; }
    };
    return false;
}

// ---------------------------------------------------------------------------------------------- //

bool IsSupported(
    Hdf::Compressor::Value value)
{
    htri_t avail = -1;
    switch(value)
    {
        case Hdf::Compressor::None:         { avail = H5Zfilter_avail(H5Z_FILTER_NONE); break; }
        case Hdf::Compressor::Deflate:      { avail = H5Zfilter_avail(H5Z_FILTER_DEFLATE); break; }
        case Hdf::Compressor::Shuffle:      { avail = H5Zfilter_avail(H5Z_FILTER_SHUFFLE); break; }
        case Hdf::Compressor::Fletcher:     { avail = H5Zfilter_avail(H5Z_FILTER_FLETCHER32); break; }
        case Hdf::Compressor::SZip:         { avail = H5Zfilter_avail(H5Z_FILTER_SZIP); break; }
        case Hdf::Compressor::NBit:         { avail = H5Zfilter_avail(H5Z_FILTER_NBIT); break; }
        case Hdf::Compressor::ScaleOffset:  { avail = H5Zfilter_avail(H5Z_FILTER_SCALEOFFSET); break; }
        default: { avail = -1; break; }
    }

    if(avail == -1)
        return false;

    return true;
}

// ---------------------------------------------------------------------------------------------- //

} // END NAMESPACE: Hdf

// ---------------------------------------------------------------------------------------------- //

// VD_IMPLEMENT_OBJECT(GadgetSnapshot, vd_sym(GadgetSnapshot), vd_sym(Object));

// ============================================================================================== //

VD_FORMATS_NAMESPACE_END();

// ============================================================================================== //

#endif // VD_USE_HDF5

// ============================================================================================== //
