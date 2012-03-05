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

#include "formats/hdf.h"
#include "constants/constants.h"

#include <string.h>
#include <math.h>

// ============================================================================================== //

VD_FORMATS_NAMESPACE_BEGIN();

// ============================================================================================== //

namespace Hdf
{

// ============================================================================================== //

bool
ReadAttribute(hid_t location, const string& attrib_name, string& value)
{
    H5T_class_t typeClass;
    H5A_info_t attrInfo;
    hsize_t strLen;

    if(H5Aexists(location, attrib_name.c_str()) < 1)
        vdException("Couldn't find attribute %s", attrib_name.c_str());

    AttribDataAccess attr(location, attrib_name.c_str(), H5P_DEFAULT);
    AttribSpaceAccess attrib_space(attr);
    AttribTypeAccess attrib_type(attr);

    if(H5Aget_info(attr, &attrInfo) < 0)
    {
        vdException("Couldn't get attribute info %s", attrib_name.c_str());
    }
    else
    {
        strLen = attrInfo.data_size;
    }

    typeClass = H5Tget_class(attrib_type);

    if(typeClass != H5T_STRING)
        vdException("Bad attribute type class for %s", attrib_name.c_str());

    NativeTypeAccess nativeType(attrib_type, H5T_DIR_ASCEND);

    std::vector<char> tempString(strLen + 1);

    if(H5Aread(attr, nativeType, &tempString[0]) < 0)
        vdException("Couldn't read attribute %s", attrib_name.c_str());

    value = string(&tempString[0]);

    return true;

}

// ============================================================================================== //

bool
ReadAttribute(hid_t location, const string& attrib_name,
              unsigned int attrSize, int& value)
{
    H5T_class_t typeClass;

    if(H5Aexists(location, attrib_name.c_str()) < 1)
        vdException("Couldn't find attribute %s", attrib_name.c_str());

    AttribDataAccess attr(location, attrib_name.c_str(), H5P_DEFAULT);
    AttribSpaceAccess attrib_space(attr);
    AttribTypeAccess attrib_type(attr);

    if(H5Sget_simple_extent_ndims(attrib_space) != 1)
        vdException("Bad attribute rank for attribute %s", attrib_name.c_str());

    hsize_t dims[1];
    H5Sget_simple_extent_dims(attrib_space, dims, NULL);

    if(dims[0] != attrSize)
        vdException("Invalid attribute size for attribute %s", attrib_name.c_str());

    typeClass = H5Tget_class(attrib_type);

    if(typeClass != H5T_INTEGER)
        vdException("Bad attribute type class for %s", attrib_name.c_str());

    NativeTypeAccess nativeType(attrib_type, H5T_DIR_ASCEND);

    if(H5Aread(attr, nativeType, &value) < 0)
        vdException("Couldn't read attribute %s", attrib_name.c_str());

    return true;

}

//----------------------------------------------------------------------------//

bool
ReadAttribute(hid_t location, const string& attrib_name,
              unsigned int attrSize, float& value)
{
    H5T_class_t typeClass;

    if(H5Aexists(location, attrib_name.c_str()) < 1)
        vdException("Couldn't find attribute %s", attrib_name.c_str());

    AttribDataAccess attr(location, attrib_name.c_str(), H5P_DEFAULT);
    AttribSpaceAccess attrib_space(attr);
    AttribTypeAccess attrib_type(attr);

    if(H5Sget_simple_extent_ndims(attrib_space) != 1)
        vdException("Bad attribute rank for attribute %s", attrib_name.c_str());

    hsize_t dims[1];
    H5Sget_simple_extent_dims(attrib_space, dims, NULL);

    if(dims[0] != attrSize)
        vdException("Invalid attribute size for attribute %s", attrib_name.c_str());

    typeClass = H5Tget_class(attrib_type);

    if(typeClass != H5T_FLOAT)
        vdException("Bad attribute type class for %s", attrib_name.c_str());

    NativeTypeAccess nativeType(attrib_type, H5T_DIR_ASCEND);

    if(H5Aread(attr, nativeType, &value) < 0)
        vdException("Couldn't read attribute %s", attrib_name.c_str());

    return true;
}

//----------------------------------------------------------------------------//

bool
ReadAttribute(hid_t location, const string& attrib_name,
              unsigned int attrSize, double& value)
{
    H5T_class_t typeClass;

    if(H5Aexists(location, attrib_name.c_str()) < 0)
        vdException("Couldn't find attribute %s", attrib_name.c_str());

    AttribDataAccess attr(location, attrib_name.c_str(), H5P_DEFAULT);
    AttribSpaceAccess attrib_space(attr);
    AttribTypeAccess attrib_type(attr);

    if(H5Sget_simple_extent_ndims(attrib_space) != 1)
        vdException("Bad attribute rank for attribute " +
                                        attrib_name);

    hsize_t dims[1];
    H5Sget_simple_extent_dims(attrib_space, dims, NULL);

    if(dims[0] != attrSize)
        vdException("Invalid attribute size for attribute " +
                                        attrib_name);

    typeClass = H5Tget_class(attrib_type);

    if(typeClass != H5T_FLOAT)
        vdException("Bad attribute type class for " +
                                        attrib_name);

    NativeTypeAccess nativeType(attrib_type, H5T_DIR_ASCEND);

    if(H5Aread(attr, nativeType, &value) < 0)
        vdException("Couldn't read attribute %s", attrib_name.c_str());

    return true;
}

//----------------------------------------------------------------------------//

bool
WriteAttribute(hid_t location, const string& attrib_name, const string& value)
{
    hid_t attr = -1;
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
        attr = H5Acreate(location, attrib_name.c_str(), attrib_type, attrib_space,
                         H5P_DEFAULT, H5P_DEFAULT);
    }

    if(attr < 0)
    {
        vdLogGlobalWarning("Error creating attribute: %s", attrib_name.c_str());
        success = false;
    }

    if(success && H5Awrite(attr, attrib_type, value.c_str()) < 0)
    {
        vdLogGlobalWarning("Error writing attribute: %s", attrib_name.c_str());
        success = false;
    }

    H5Aclose(attr);
    H5Tclose(attrib_type);
    H5Sclose(attrib_space);

    return success;

}

//----------------------------------------------------------------------------//

bool
WriteAttribute(hid_t location, const string& attrib_name,
               unsigned int attrSize, const int& value)
{
    hid_t attr;
    hid_t attrib_space;
    hsize_t dims[1];

    dims[0] = attrSize;

    attrib_space = H5Screate(H5S_SIMPLE);
    if(attrib_space < 0)
        return false;

    if(H5Sset_extent_simple(attrib_space, 1, dims, NULL) < 0)
        return false;

    attr = H5Acreate(location, attrib_name.c_str(), H5T_NATIVE_INT,
                     attrib_space, H5P_DEFAULT, H5P_DEFAULT);

    if(attr < 0)
    {
        vdLogGlobalWarning("Error creating attribute: %s", attrib_name.c_str());
        H5Aclose(attr);
        H5Sclose(attrib_space);
        return false;
    }

    if(H5Awrite(attr, H5T_NATIVE_INT, &value) < 0)
    {
        vdLogGlobalWarning("Error writing attribute: %s", attrib_name.c_str());
        H5Aclose(attr);
        H5Sclose(attrib_space);
        return false;
    }

    H5Aclose(attr);
    H5Sclose(attrib_space);

    return true;
}

//----------------------------------------------------------------------------//

bool
WriteAttribute(hid_t location, const string& attrib_name,
               unsigned int attrSize, const float& value)
{
    hid_t attr;
    hid_t attrib_space;
    hsize_t dims[1];

    dims[0] = attrSize;

    attrib_space = H5Screate(H5S_SIMPLE);
    if(attrib_space < 0)
        return false;

    if(H5Sset_extent_simple(attrib_space, 1, dims, NULL) < 0)
        return false;

    attr = H5Acreate(location, attrib_name.c_str(), H5T_NATIVE_FLOAT,
                     attrib_space, H5P_DEFAULT, H5P_DEFAULT);

    if(attr < 0)
    {
        vdLogGlobalWarning("Error creating attribute: %s", attrib_name.c_str());
        H5Aclose(attr);
        H5Sclose(attrib_space);
        return false;
    }

    if(H5Awrite(attr, H5T_NATIVE_FLOAT, &value) < 0)
    {
        vdLogGlobalWarning("Error writing attribute: %s", attrib_name.c_str());
        H5Aclose(attr);
        H5Sclose(attrib_space);
        return false;
    }

    H5Aclose(attr);
    H5Sclose(attrib_space);

    return true;
}

//----------------------------------------------------------------------------//

bool
WriteAttribute(hid_t location, const string& attrib_name,
               unsigned int attrSize, const double& value)
{
    hid_t attr;
    hid_t attrib_space;
    hsize_t dims[1];

    dims[0] = attrSize;

    attrib_space = H5Screate(H5S_SIMPLE);

    if(attrib_space < 0)
        return false;

    if(H5Sset_extent_simple(attrib_space, 1, dims, NULL) < 0)
        return false;

    attr = H5Acreate(location, attrib_name.c_str(), H5T_NATIVE_DOUBLE,
                     attrib_space, H5P_DEFAULT, H5P_DEFAULT);

    if(attr < 0)
    {
        vdLogGlobalWarning("Error creating attribute: %s", attrib_name.c_str());
        H5Aclose(attr);
        H5Sclose(attrib_space);
        return false;
    }

    if(H5Awrite(attr, H5T_NATIVE_DOUBLE, &value) < 0)
    {
        vdLogGlobalWarning("Error writing attribute: %s", attrib_name.c_str());
        H5Aclose(attr);
        H5Sclose(attrib_space);
        return false;
    }

    H5Aclose(attr);
    H5Sclose(attrib_space);

    return true;
}

//----------------------------------------------------------------------------//

bool IsSupported(
    Hdf::Feature::Value value)
{
    if(value == Hdf::Feature::Gzip)
    {
        htri_t avail = H5Zfilter_avail(H5Z_FILTER_DEFLATE);

        if(!avail)
            return false;

        unsigned int filter_info;
        herr_t status = H5Zget_filter_info(H5Z_FILTER_DEFLATE, &filter_info);

        if(status < 0)
            return false;

        if(!(filter_info & H5Z_FILTER_CONFIG_ENCODE_ENABLED) ||
                !(filter_info & H5Z_FILTER_CONFIG_DECODE_ENABLED))
        {
            return false;
        }

        return true;
    }

    return false;
}

// ============================================================================================== //

} // END NAMESPACE: Hdf

// ============================================================================================== //

// VD_IMPLEMENT_OBJECT(GadgetSnapshot, vd_sym(GadgetSnapshot), vd_sym(Object));

// ============================================================================================== //

VD_FORMATS_NAMESPACE_END();

// ============================================================================================== //

#endif // VD_USE_GS2

// ============================================================================================== //
