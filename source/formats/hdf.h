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

#include "formats/formats.h"
#include "core/core.h"
#include "core/status.h"
#include "core/entity.h"
#include "core/memory.h"
#include "core/atomics.h"
#include "core/workqueue.h"
#include "runtime/runtime.h"
#include "runtime/context.h"
#include "containers/cache.h"

// ============================================================================================== //

VD_FORMATS_NAMESPACE_BEGIN();

// ============================================================================================== //

#include <hdf5.h>

// ============================================================================================== //

namespace Hdf
{

// ============================================================================================== //

class Entity
{
public:
    Entity() : m_HdfId(-1)   { }
    hid_t GetHdfId() const   { return m_HdfId; }
    operator hid_t()         { return m_HdfId; }
protected:
    hid_t m_HdfId;
};

// ---------------------------------------------------------------------------------------------- //

class AttriDataAccess : public Hdf::Entity
{
public:

    AttriDataAccess(hid_t location, const vd::string& name)
    {
        m_HdfId = H5Aopen(location, name.c_str(), H5P_DEFAULT);
        if(m_HdfId < 0)
            vdLogGlobalError("Couldn't open attribute " + name);
    }

    AttriDataAccess(hid_t location, const vd::string& name, hid_t aapl_id)
    {
        m_HdfId = H5Aopen(location, name.c_str(), aapl_id);

        if(m_HdfId < 0)
            vdLogGlobalError("Couldn't open attribute " + name);
    }

    ~AttriDataAccess()
    {
        if(m_HdfId >= 0)
            H5Aclose(m_HdfId);
    }
};

// ---------------------------------------------------------------------------------------------- //

class ScopedAttributeIdx : public Hdf::Entity
{
public:
    ScopedAttributeIdx(hid_t location, unsigned idx)
    {
        m_HdfId = H5Aopen_idx(location, idx);

        if(m_HdfId < 0)
        {
            vdLogGlobalError("Couldn't open attribute at index: ", ToString(idx));
        }
    }
    ~ScopedAttributeIdx()
    {
        if(m_HdfId >= 0)
            H5Aclose(m_HdfId);
    }
};

// ---------------------------------------------------------------------------------------------- //

class GroupCreate : public Hdf::Entity
{
public:
    GroupCreate(hid_t parentLocation, const vd::string& name)
    {
        m_HdfId = H5Gcreate(parentLocation, name.c_str(),
                         H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    }
    GroupCreate(hid_t parentLocation, const vd::string& name,
                    hid_t lcpl_id, hid_t gcpl_id, hid_t gapl_id)
    {
        m_HdfId = H5Gcreate(parentLocation, name.c_str(),
                         lcpl_id, gcpl_id, gapl_id);
    }

    ~GroupCreate()
    {
        if(m_HdfId >= 0)
            H5Gclose(m_HdfId);
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
    GroupAccess(hid_t parentLocation, const vd::string& name)
    {
        Open(parentLocation, name);
    }
    GroupAccess(hid_t parentLocation, const vd::string& name, hid_t gapl_id)
    {
        Open(parentLocation, name, gapl_id);
    }
    void Open(hid_t parentLocation, const vd::string& name)
    {
        m_HdfId = H5Gopen(parentLocation, name.c_str(), H5P_DEFAULT);
    }
    void Open(hid_t parentLocation, const vd::string& name, hid_t gapl_id)
    {
        m_HdfId = H5Gopen(parentLocation, name.c_str(), gapl_id);
    }

    ~GroupAccess()
    {
        if(m_HdfId >= 0)
            H5Gclose(m_HdfId);
    }
};

// ---------------------------------------------------------------------------------------------- //

class SpaceCreate : public Hdf::Entity
{
public:
    SpaceCreate()
        : Hdf::Entity()
    {
        // Empty
    }
    SpaceCreate(H5S_class_t type)
    {
        Create(type);
    }
    void Create(H5S_class_t type)
    {
        m_HdfId = H5Screate(type);
    }
    ~SpaceCreate()
    {
        if(m_HdfId >= 0)
            H5Sclose(m_HdfId);
    }
};

// ---------------------------------------------------------------------------------------------- //

class DatasetCreate : public Hdf::Entity
{
public:
    DatasetCreate(hid_t parentLocation, const vd::string& name,
                    hid_t dtype_id, hid_t space_id, hid_t lcpl_id,
                    hid_t dcpl_id, hid_t dapl_id)
    {
        m_HdfId = H5Dcreate(parentLocation, name.c_str(), dtype_id, space_id,
                         lcpl_id, dcpl_id, dapl_id);
    }
    ~DatasetCreate()
    {
        if(m_HdfId >= 0)
            H5Dclose(m_HdfId);
    }
};

// ---------------------------------------------------------------------------------------------- //

class AttribSpaceAccess : public Hdf::Entity
{
public:
    AttribSpaceAccess(hid_t dataset_id)
    {
        m_HdfId = H5Aget_space(dataset_id);

        if(m_HdfId < 0)
            vdLogGlobalError("Couldn't get attribute space");
    }
    ~AttribSpaceAccess()
    {
        if(m_HdfId >= 0)
            H5Sclose(m_HdfId);
    }
};

// ---------------------------------------------------------------------------------------------- //

class AttribTypeAccess : public Hdf::Entity
{
public:
    AttribTypeAccess(hid_t dataset_id)
    {
        m_HdfId = H5Aget_type(dataset_id);

        if(m_HdfId < 0)
           vdLogGlobalError("Couldn't get attribute type");
    }
    ~AttribTypeAccess()
    {
        if(m_HdfId >= 0)
            H5Tclose(m_HdfId);
    }
};

// ---------------------------------------------------------------------------------------------- //

class NativeTypeAccess : public Hdf::Entity
{
public:
    NativeTypeAccess(hid_t dataset_id, H5T_direction_t direction)
    {
        m_HdfId = H5Tget_native_type(dataset_id, direction);

        if(m_HdfId < 0)
            vdLogGlobalError("Couldn't get native attribute type");
    }
    ~NativeTypeAccess()
    {
        if(m_HdfId >= 0)
            H5Tclose(m_HdfId);
    }
};

// ---------------------------------------------------------------------------------------------- //

class DatasetAccess : public Hdf::Entity
{
public:
    DatasetAccess()
        : Hdf::Entity()
    {
        // Empty
    }
    DatasetAccess(hid_t parentLocation, const vd::string& name, hid_t dapl_id)
    {
        Open(parentLocation, name, dapl_id);
    }
    void Open(hid_t parentLocation, const vd::string& name, hid_t dapl_id)
    {
        m_HdfId = H5Dopen(parentLocation, name.c_str(), dapl_id);
    }
    ~DatasetAccess()
    {
        if(m_HdfId >= 0)
        {
            H5Dclose(m_HdfId);
        }
    }
};

// ---------------------------------------------------------------------------------------------- //

class DatasetSpaceAccess : public Hdf::Entity
{
public:
    DatasetSpaceAccess()
        : Hdf::Entity()
    {
        // Empty
    }
    DatasetSpaceAccess(hid_t dataset_id)
    {
        Open(dataset_id);
    }
    void Open(hid_t dataset_id)
    {
        m_HdfId = H5Dget_space(dataset_id);
    }
    ~DatasetSpaceAccess()
    {
        if(m_HdfId >= 0)
            H5Sclose(m_HdfId);
    }
};

// ---------------------------------------------------------------------------------------------- //

class DatasetTypeAccess : public Hdf::Entity
{
public:
    DatasetTypeAccess()
        : Hdf::Entity()
    {
        // Empty
    }
    DatasetTypeAccess(hid_t dataset_id)
    {
        Open(dataset_id);
    }
    void Open(hid_t dataset_id)
    {
        m_HdfId = H5Dget_type(dataset_id);
    }
    ~DatasetTypeAccess()
    {
        if(m_HdfId >= 0)
            H5Tclose(m_HdfId);
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
    hid_t location, c
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

bool IsSupported(Hdf::Support::Value value);

// ============================================================================================== //
// Templated functions and classes
// ============================================================================================== //

template <typename T>
void WriteData(
    hid_t location,
    const vd::string& name,
    const std::vector<T> &data)
{
    hsize_t totalSize[1];
    int components = Traits<T>::Dimensions();
    totalSize[0] = data.size() * components;

    // Get the internal data type
    hid_t type = DataTypeTraits<T>::Type();

    ScopedSpaceCreate dataSpace(H5S_SIMPLE);

    if(dataSpace.GetHdfId() < 0)
        vdLogGlobalError("Couldn't create data space");

    H5Sset_extent_simple(dataSpace.GetHdfId(), 1, totalSize, NULL);

    ScopedDatasetCreate dataset(location, name.c_str(), type, dataSpace.GetHdfId(),
                            H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

    if(dataset.GetHdfId() < 0)
        vdLogGlobalError("Couldn't create data set");

    hid_t err = H5Dwrite(dataset.GetHdfId(), type, H5S_ALL, H5S_ALL,
                         H5P_DEFAULT, &data[0]);

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
    int components = FieldTraits<T>::dataDims();
    hsize_t dims[1];

    DatasetAccess dataset(location, name.c_str(), H5P_DEFAULT);

    if(dataset.GetHdfId() < 0)
        vdLogGlobalError("Couldn't open data set: " + name);

    DatasetSpaceAccess dataSpace(dataset.GetHdfId());
    DatasetTypeAccess dataType(dataset.GetHdfId());
    H5Sget_simple_extent_dims(dataSpace.GetHdfId(), dims, NULL);

    if(dataSpace.GetHdfId() < 0)
        vdLogGlobalError("Couldn't get data space");

    if(dataType.GetHdfId() < 0)
        vdLogGlobalError("Couldn't get data type");

    int reportedSize = dims[0] / components;

    // Resize target
    data.clear();
    data.resize(reportedSize);

    // Get the internal data type
    hid_t type = DataTypeTraits<T>::Type();

    if(H5Dread(dataset.GetHdfId(), type, H5S_ALL, H5S_ALL,
               H5P_DEFAULT, &data[0]) < 0)
    {
        vdLogGlobalError("Couldn't read simple data");
    }
}

// ============================================================================================== //

VD_FORMATS_NAMESPACE_END();

// ============================================================================================== //

#endif // VD_FORMATS_HDF5_HEADER_INCLUDED

// ============================================================================================== //

#endif // VD_USE_HDF5

// ============================================================================================== //
// END OF FILE

