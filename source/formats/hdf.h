// ============================================================================================== //
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
// ============================================================================================== //

#if defined(VD_USE_HDF5)

// ============================================================================================== //

#ifndef VD_FORMATS_HDF5_HEADER_INCLUDED
#define VD_FORMATS_HDF5_HEADER_INCLUDED

// ============================================================================================== //

#include <hdf5.h>
#include "vd.h"
#include "formats/formats.h"
#include "core/core.h"
#include "core/status.h"
#include "core/entity.h"
#include "core/memory.h"
#include "core/atomics.h"
#include "core/utilities.h"
#include "core/workqueue.h"
#include "runtime/runtime.h"
#include "runtime/context.h"
#include "containers/cache.h"

// ============================================================================================== //

VD_FORMATS_NAMESPACE_BEGIN();

// ============================================================================================== //

namespace Hdf
{

// ============================================================================================== //

// Built-in compression modules
VD_DECLARE_ENUM(Compressor,
    None, 
    Deflate,
    Shuffle,
    Fletcher,
    SZip,
    NBit,
    ScaleOffset);

// Compile time enabled features
VD_DECLARE_ENUM(Feature,
    ZLib,
    SZip,
    Parallel,
    MPI);

// ============================================================================================== //

template <typename T>
struct TypeConverter 
{  
    static hid_t ToNative() 
    {
        return 0;
    }
};

template <> inline hid_t TypeConverter<vd::i8>::ToNative()       { return H5T_NATIVE_CHAR;  }
template <> inline hid_t TypeConverter<vd::i16>::ToNative()      { return H5T_NATIVE_SHORT; }
template <> inline hid_t TypeConverter<vd::i32>::ToNative()      { return H5T_NATIVE_INT; }
template <> inline hid_t TypeConverter<vd::i64>::ToNative()      { return H5T_NATIVE_LONG; }
template <> inline hid_t TypeConverter<vd::u8>::ToNative()       { return H5T_NATIVE_UCHAR; }
template <> inline hid_t TypeConverter<vd::u16>::ToNative()      { return H5T_NATIVE_USHORT; }
template <> inline hid_t TypeConverter<vd::u32>::ToNative()      { return H5T_NATIVE_UINT; }
template <> inline hid_t TypeConverter<vd::u64>::ToNative()      { return H5T_NATIVE_ULONG; }
template <> inline hid_t TypeConverter<vd::f16>::ToNative()      { return H5T_NATIVE_USHORT; }
template <> inline hid_t TypeConverter<vd::f32>::ToNative()      { return H5T_NATIVE_FLOAT; }
template <> inline hid_t TypeConverter<vd::f64>::ToNative()      { return H5T_NATIVE_DOUBLE; }

// ============================================================================================== //

class Entity
{
public:
    Entity() : m_Hid(-1)    { }
    hid_t GetHid() const    { return m_Hid; }
    operator hid_t()        { return m_Hid; }
protected:
    hid_t m_Hid;
};

// ---------------------------------------------------------------------------------------------- //

class AttribDataAccess : public Hdf::Entity
{
public:

    AttribDataAccess(
        hid_t location, 
        const vd::string& name)
    {
        m_Hid = H5Aopen(location, name.c_str(), H5P_DEFAULT);
        if(m_Hid < 0)
            vdLogGlobalError("Couldn't open attribute: '%s'", name.c_str());
    }

    AttribDataAccess(
        hid_t location, 
        const vd::string& name, 
        hid_t aapl_id)
    {
        m_Hid = H5Aopen(location, name.c_str(), aapl_id);

        if(m_Hid < 0)
            vdLogGlobalError("Couldn't open attribute: '%s'", name.c_str());
    }

    ~AttribDataAccess()
    {
        if(m_Hid >= 0)
            H5Aclose(m_Hid);
    }
};

// ---------------------------------------------------------------------------------------------- //

class AttribIdxAccess : public Hdf::Entity
{
public:

    AttribIdxAccess(
        hid_t location, 
        unsigned idx)
    {
        m_Hid = H5Aopen_idx(location, idx);

        if(m_Hid < 0)
        {
            vdLogGlobalError("Couldn't open attribute at index: '%d'", idx);
        }
    }
    ~AttribIdxAccess()
    {
        if(m_Hid >= 0)
            H5Aclose(m_Hid);
    }
};

// ---------------------------------------------------------------------------------------------- //

class GroupCreate : public Hdf::Entity
{
public:
    
    GroupCreate(
        hid_t location, 
        const vd::string& name)
    {
        m_Hid = H5Gcreate(location, name.c_str(), H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    }

    GroupCreate(hid_t location, const vd::string& name, hid_t lcpl_id, hid_t gcpl_id, hid_t gapl_id)
    {
        m_Hid = H5Gcreate(location, name.c_str(), lcpl_id, gcpl_id, gapl_id);
    }

    ~GroupCreate()
    {
        if(m_Hid >= 0)
            H5Gclose(m_Hid);
    }
};

// ---------------------------------------------------------------------------------------------- //

class GroupAccess : public Hdf::Entity
{
public:

    GroupAccess()
        : Hdf::Entity()
    {
        // Empty
    }
    
    GroupAccess(hid_t location, const vd::string& name)
    {
        Open(location, name);
    }
    
    GroupAccess(hid_t location, const vd::string& name, hid_t gapl_id)
    {
        Open(location, name, gapl_id);
    }
    
    void Open(hid_t location, const vd::string& name)
    {
        m_Hid = H5Gopen(location, name.c_str(), H5P_DEFAULT);
    }
    
    void Open(hid_t location, const vd::string& name, hid_t gapl_id)
    {
        m_Hid = H5Gopen(location, name.c_str(), gapl_id);
    }

    ~GroupAccess()
    {
        if(m_Hid >= 0)
            H5Gclose(m_Hid);
    }
};

// ---------------------------------------------------------------------------------------------- //

class SpaceCreator : public Hdf::Entity
{
public:
    SpaceCreator()
        : Hdf::Entity()
    {
        // Empty
    }
    SpaceCreator(H5S_class_t type)
    {
        Create(type);
    }
    void Create(H5S_class_t type)
    {
        m_Hid = H5Screate(type);
    }
    ~SpaceCreator()
    {
        if(m_Hid >= 0)
            H5Sclose(m_Hid);
    }
};

// ---------------------------------------------------------------------------------------------- //

class DataCreator : public Hdf::Entity
{
public:
    DataCreator(
        hid_t location, 
        const vd::string& name,
        hid_t dtype_id, hid_t space_id, hid_t lcpl_id,
        hid_t dcpl_id, hid_t dapl_id)
    {
        m_Hid = H5Dcreate(location, name.c_str(), dtype_id, space_id, lcpl_id, dcpl_id, dapl_id);
    }
    ~DataCreator()
    {
        if(m_Hid >= 0)
            H5Dclose(m_Hid);
    }
};

// ---------------------------------------------------------------------------------------------- //

class AttribSpaceAccess : public Hdf::Entity
{
public:
    AttribSpaceAccess(hid_t dataset_id)
    {
        m_Hid = H5Aget_space(dataset_id);
        if(m_Hid < 0)
            vdLogGlobalError("Couldn't get attribute space for dataset: '%d'", (int)dataset_id);
    }
    ~AttribSpaceAccess()
    {
        if(m_Hid >= 0)
            H5Sclose(m_Hid);
    }
};

// ---------------------------------------------------------------------------------------------- //

class AttribTypeAccess : public Hdf::Entity
{
public:
    AttribTypeAccess(hid_t dataset_id)
    {
        m_Hid = H5Aget_type(dataset_id);
        if(m_Hid < 0)
           vdLogGlobalError("Couldn't get attribute type: '%d'", (int)dataset_id);
    }
    ~AttribTypeAccess()
    {
        if(m_Hid >= 0)
            H5Tclose(m_Hid);
    }
};

// ---------------------------------------------------------------------------------------------- //

class NativeTypeAccess : public Hdf::Entity
{
public:
    NativeTypeAccess(hid_t dataset_id, H5T_direction_t direction)
    {
        m_Hid = H5Tget_native_type(dataset_id, direction);
        if(m_Hid < 0)
            vdLogGlobalError("Couldn't get native attribute type: '%d'", (int)dataset_id);
    }
    ~NativeTypeAccess()
    {
        if(m_Hid >= 0)
            H5Tclose(m_Hid);
    }
};

// ---------------------------------------------------------------------------------------------- //

class DataAccess : public Hdf::Entity
{
public:
    DataAccess()
        : Hdf::Entity()
    {
        // Empty
    }
    DataAccess(hid_t location, const vd::string& name, hid_t dapl_id)
    {
        Open(location, name, dapl_id);
    }
    void Open(hid_t location, const vd::string& name, hid_t dapl_id)
    {
        m_Hid = H5Dopen(location, name.c_str(), dapl_id);
    }
    ~DataAccess()
    {
        if(m_Hid >= 0)
        {
            H5Dclose(m_Hid);
        }
    }
};

// ---------------------------------------------------------------------------------------------- //

class DataSpaceAccess : public Hdf::Entity
{
public:
    DataSpaceAccess()
        : Hdf::Entity()
    {
        // Empty
    }
    DataSpaceAccess(hid_t dataset_id)
    {
        Open(dataset_id);
    }
    void Open(hid_t dataset_id)
    {
        m_Hid = H5Dget_space(dataset_id);
    }
    ~DataSpaceAccess()
    {
        if(m_Hid >= 0)
            H5Sclose(m_Hid);
    }
};

// ---------------------------------------------------------------------------------------------- //

class DataTypeAccess : public Hdf::Entity
{
public:
    DataTypeAccess()
        : Hdf::Entity()
    {
        // Empty
    }
    DataTypeAccess(hid_t dataset_id)
    {
        Open(dataset_id);
    }
    void Open(hid_t dataset_id)
    {
        m_Hid = H5Dget_type(dataset_id);
    }
    ~DataTypeAccess()
    {
        if(m_Hid >= 0)
            H5Tclose(m_Hid);
    }
};

// ---------------------------------------------------------------------------------------------- //

template <typename T>
void WriteData(
    hid_t location,
    const vd::string& name,
    const std::vector<T> &data);

template <typename T>
void ReadData(
    hid_t location,
    const vd::string& name,
    std::vector<T> &data);

bool ReadAttribute(
    hid_t location,
    const vd::string& name,
    vd::string& value);

bool ReadAttribute(
    hid_t location,
    const vd::string& name,
    unsigned int size,
    int& value);

bool ReadAttribute(
    hid_t location,
    const vd::string& name,
    unsigned int size,
    float& value);

bool ReadAttribute(
    hid_t location,
    const vd::string& name,
    unsigned int size,
    double& value);

bool WriteAttribute(
    hid_t location,
    const vd::string& attrName,
    const vd::string& value);

bool WriteAttribute(
    hid_t location,
    const vd::string& attrName,
    unsigned int attrSize,
    const int& value);

bool WriteAttribute(
    hid_t location,
    const vd::string& attrName,
    unsigned int attrSize,
    const float& value);

bool WriteAttribute(
    hid_t location,
    const vd::string& attrName,
    unsigned int attrSize,
    const double& value);

// ---------------------------------------------------------------------------------------------- //

bool IsSupported(Hdf::Feature::Value value);
bool IsSupported(Hdf::Compressor::Value value);

// ============================================================================================== //
// Templated functions and classes
// ============================================================================================== //

template <typename T>
void WriteData(
    hid_t location,
    const vd::string& name,
    const std::vector<T> &data)
{
    hsize_t total_bytes[1];
    int components = Core::TypeTraits<T>::GetComponents();
    total_bytes[0] = data.size() * components;
    hid_t type = Hdf::TypeConverter<T>::ToNative();

    SpaceCreator data_space(H5S_SIMPLE);

    if(data_space.GetHid() < 0)
        vdLogGlobalError("Couldn't create data space");

    H5Sset_extent_simple(data_space.GetHid(), 1, total_bytes, NULL);

    DataCreator data_access(location, name.c_str(), type, data_space.GetHid(),
                            H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

    if(data_access.GetHid() < 0)
        vdLogGlobalError("Couldn't create data set");

    hid_t err = H5Dwrite(data_access.GetHid(), type, H5S_ALL, H5S_ALL, H5P_DEFAULT, &data[0]);

    if(err < 0)
        vdLogGlobalError("Couldn't write data");
}

// ============================================================================================== //

template <typename T>
void ReadData(
    hid_t location,
    const vd::string& name,
    std::vector<T> &data)
{
    int components = Core::TypeTraits<T>::GetComponents();
    hsize_t dims[1];

    DataAccess data_access(location, name.c_str(), H5P_DEFAULT);
    if(data_access.GetHid() < 0)
        vdLogGlobalError("Couldn't open data set: '%s'", name.c_str());

    DataSpaceAccess data_space(data_access.GetHid());
    DataTypeAccess data_type(data_access.GetHid());
    H5Sget_simple_extent_dims(data_space.GetHid(), dims, NULL);

    if(data_space.GetHid() < 0)
        vdLogGlobalError("Couldn't get data space: '%s'", name.c_str());

    if(data_type.GetHid() < 0)
        vdLogGlobalError("Couldn't get data type: '%s'", name.c_str());

    int result_size = dims[0] / components;

    data.clear();
    data.resize(result_size);

    hid_t type = Hdf::TypeConverter<T>::ToNative();
    if(H5Dread(data_access.GetHid(), type, H5S_ALL, H5S_ALL, H5P_DEFAULT, &data[0]) < 0)
    {
        vdLogGlobalError("Couldn't read simple data");
    }
}

// ---------------------------------------------------------------------------------------------- //

}   // END NAMESPACE: Hdf

// ---------------------------------------------------------------------------------------------- //

VD_FORMATS_NAMESPACE_END();

// ============================================================================================== //

#endif // VD_FORMATS_HDF5_HEADER_INCLUDED

// ============================================================================================== //

#endif // VD_USE_HDF5

// ============================================================================================== //
// END OF FILE

