// ============================================================================================== //
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
// ============================================================================================== //

#include "hdf.h"

// ============================================================================================== //

namespace Storage {
namespace Hdf {

// ============================================================================================== //

std::string
Basename(const std::string& name)
{
    std::string base;
    if(name.length() < 1)
        return base;

    size_t j = name.length() - 1;
    while(name[j] != '/' && j != 0)
        j--;

    base = std::string(name.c_str() + j + 1);
    return base;
}

// ============================================================================================== //

Access::Access()         
{ 
    Invalidate();
}

Access::Access(hid_t h) : m_Hid(h)    
{ 
    // EMPTY
}

Access::~Access()
{
    // EMPTY
}

bool Access::IsValid() const            
{ 
    return (m_Hid == -1) ? false : true; 
}

bool Access::IsValid(hid_t v) 
{ 
    return (v == -1) ? false : true; 
}

Access::operator hid_t() const
{ 
    return m_Hid; 
}

void Access::Invalidate()               
{ 
    m_Hid = -1; 
}

AccessClassId
Access::GetAccessClassId() const
{
    return VD_ACCESS_CLASS_ANY;
}

// ---------------------------------------------------------------------------------------------- //

InvalidAccess::InvalidAccess() : Access()
{
    Invalidate();
}

InvalidAccess::~InvalidAccess()
{
    Invalidate();
}

AccessClassId
InvalidAccess::GetAccessClassId() const
{
    return VD_ACCESS_CLASS_INVALID;
}

// ---------------------------------------------------------------------------------------------- //

PropertyListAccess::PropertyListAccess() 
    : Hdf::Access(H5P_DEFAULT)
{
    // EMPTY
}

PropertyListAccess::~PropertyListAccess() 
{ 
    Close(); 
}

bool 
PropertyListAccess::Open(
    const FileAccess& access, PropertyListTypeId type )
{
    switch(type)
    {
        case VD_PROPERTY_LIST_CREATE: { m_Hid = H5Fget_create_plist( access ); }
        case VD_PROPERTY_LIST_ACCESS: { m_Hid = H5Fget_access_plist( access ); }
        default:                      { Hdf::Access::Invalidate(); }
    }

    if(Hdf::Access::IsValid(m_Hid) == false)
    {
        vdLogGlobalError("Failed to open property list: '%s'", access.GetFileName().c_str());
        return false;
    }
    return true;
}

bool 
PropertyListAccess::Open(
    const DataSetAccess& access, PropertyListTypeId type )
{
    switch(type)
    {
        case VD_PROPERTY_LIST_CREATE: { m_Hid = H5Dget_create_plist( access ); }
        case VD_PROPERTY_LIST_ACCESS: { m_Hid = H5Dget_access_plist( access ); }
        default:                      { Hdf::Access::Invalidate(); }
    }

    if(Hdf::Access::IsValid(m_Hid) == false)
    {
        vdLogGlobalError("Failed to open property list: '%s'", access.GetName().c_str());
        return false;
    }
    return true;
}

bool 
PropertyListAccess::Open(
    const AttribListAccess& access, PropertyListTypeId type )
{
    switch(type)
    {
        case VD_PROPERTY_LIST_CREATE: { m_Hid = H5Dget_create_plist( access ); }
        default:                      { Hdf::Access::Invalidate(); }
    }

    if(Hdf::Access::IsValid(m_Hid) == false)
    {
        vdLogGlobalError("Failed to open property list: '%s'", access.GetName().c_str());
        return false;
    }
    return true;
}

bool 
PropertyListAccess::Open(
    const GroupAccess& access, PropertyListTypeId type )
{
    switch(type)
    {
        case VD_PROPERTY_LIST_CREATE: { m_Hid = H5Gget_create_plist( access ); }
        default:                      { Hdf::Access::Invalidate(); }
    }

    if(Hdf::Access::IsValid(m_Hid) == false)
    {
        vdLogGlobalError("Failed to open property list: '%s'", access.GetName().c_str());
        return false;
    }
    return true;
}

bool 
PropertyListAccess::Close()
{
    if(!IsValid()) return false;
    herr_t error = H5Pclose(m_Hid);
    if(error) vdLogGlobalError("Failed close property list!");
    Hdf::Access::Invalidate();
    return (error) ? false : true;
}

AccessClassId 
PropertyListAccess::GetAccessClassId() const
{
    return VD_ACCESS_CLASS_PROPERTY_LIST;
}

// ---------------------------------------------------------------------------------------------- //

TypeAccess::TypeAccess()
    : Hdf::Access()
{
    // EMPTY!
}

TypeAccess::~TypeAccess() 
{ 
    Close(); 
}

bool 
TypeAccess::Open(
    const Hdf::DataSetAccess& location)
{
    Close();
    m_Hid = H5Dget_type(location);
    if(Hdf::Access::IsValid(m_Hid) == false)
    {
        vdLogGlobalError("Failed to open type access!");
        return false;
    }

    return true;
}

bool 
TypeAccess::Open(
    const Hdf::AttribValueAccess& location)
{
    Close();
    m_Hid = H5Aget_type(location);
    if(Hdf::Access::IsValid(m_Hid) == false)
    {
        vdLogGlobalError("Failed to open type access!");
        return false;
    }

    return true;
}

bool
TypeAccess::Open(    
    const Hdf::Access& location, 
    const std::string& name)
{
    Close();
    m_Hid = H5Topen(location, name.c_str(), H5P_DEFAULT);    
    if(Hdf::Access::IsValid(m_Hid) == false)
    {
        vdLogGlobalError("Failed to open type access: '%s'", name.c_str());
        return false;
    }
    m_Name = name;
    return true;
}

bool 
TypeAccess::Open(
    const Hdf::Access& location, 
    const std::string& name, 
    const Hdf::PropertyListAccess& aapl)
{
    Close();
    m_Hid = H5Topen(location, name.c_str(), aapl);
    if(Hdf::Access::IsValid(m_Hid) == false)
    {
        vdLogGlobalError("Failed to open type access: '%s'", name.c_str());
        return false;
    }
    m_Name = name;
    return true;
}

bool 
TypeAccess::Close()
{
    if(!IsValid()) return false;
    herr_t error = H5Tclose(m_Hid);
    if(error) vdLogGlobalError("Failed close type access!");
    Hdf::Access::Invalidate();
    return (error) ? false : true;
}

TypeClassId
TypeAccess::GetTypeClassId() const
{
    H5T_class_t type_class;
    type_class = H5Tget_class(m_Hid);
    if(type_class < 0)
    {
        return VD_TYPE_CLASS_INVALID;
    }
    else if(type_class == H5T_INTEGER)
    {
        return VD_TYPE_CLASS_INTEGER;
    }
    else if(type_class == H5T_FLOAT)
    {
        return VD_TYPE_CLASS_FLOAT;
    }
    else if(type_class == H5T_STRING)
    {
        return VD_TYPE_CLASS_STRING;
    }
    else if(type_class == H5T_BITFIELD)
    {
        return VD_TYPE_CLASS_BITFIELD;
    }
    else if(type_class == H5T_OPAQUE)
    {
        return VD_TYPE_CLASS_OPAQUE;
    }
    else if(type_class == H5T_COMPOUND)
    {
        return VD_TYPE_CLASS_COMPOUND;
    }
    else if(type_class == H5T_ARRAY)
    {
        return VD_TYPE_CLASS_ARRAY;
    }
    else if(type_class == H5T_ENUM)
    {
        return VD_TYPE_CLASS_ENUM;
    }
    return VD_TYPE_CLASS_UNKNOWN;
}

Hdf::TypeAccess
TypeAccess::GetNativeType() const
{
    hid_t native_type;
    H5T_class_t type_class;

    type_class = H5Tget_class(m_Hid);
    if(type_class == H5T_BITFIELD)
    {
        native_type = H5Tcopy(m_Hid);
    }
    else
    {
        native_type = H5Tget_native_type(m_Hid, H5T_DIR_DEFAULT);
    }
    TypeAccess native_access;
    native_access.m_Hid = native_type;
    return native_access;
}

size_t
TypeAccess::GetSizeInBytes() const
{
    return H5Tget_size(m_Hid);
}

const std::string&
TypeAccess::GetName() const
{
    return m_Name;    
}

AccessClassId 
TypeAccess::GetAccessClassId() const
{
    return VD_ACCESS_CLASS_TYPE;
}

// ---------------------------------------------------------------------------------------------- //

FileAccess::FileAccess() 
    : Hdf::Access()
{
    // EMPTY
}

FileAccess::~FileAccess()
{ 
    Close(); 
}

bool 
FileAccess::Open(
    const std::string& filename, AccessModeId mode)
{
    Close();
    uint32_t access_flag = AdaptFileAccessMode::ToAccessFlag(mode);
    m_Hid = H5Fopen(filename.c_str(), access_flag, H5P_DEFAULT);
    if(Hdf::Access::IsValid(m_Hid) == false)
    {
        vdLogGlobalError("Failed to open file: '%s'", m_FileName.c_str());
        return false;
    }
    m_FileName = filename;
    return true;
}

bool 
FileAccess::Close()
{
    if(!IsValid()) return false;
    herr_t error = H5Fclose(m_Hid);
    if(error) vdLogGlobalError("Couldn't close file: '%s'", m_FileName.c_str());
    Hdf::Access::Invalidate();
    return (error) ? false : true;
}

const std::string&
FileAccess::GetFileName() const
{
    return m_FileName;    
}

AccessClassId 
FileAccess::GetAccessClassId() const
{
    return VD_ACCESS_CLASS_FILE;
}

// ---------------------------------------------------------------------------------------------- //

GroupAccess::GroupAccess() 
    : Hdf::Access()
{
    // EMPTY
}

GroupAccess::~GroupAccess() 
{ 
    Close(); 
}

bool 
GroupAccess::Open(
    const Hdf::Access& location, 
    const std::string& name)
{
    Close();
    m_Hid = H5Gopen(location, name.c_str(), H5P_DEFAULT);
    if(Hdf::Access::IsValid(m_Hid) == false)
    {
        vdLogGlobalError("Failed to open group: '%s'", name.c_str());
        return false;
    }
    m_Name = name;
    return true;
}

bool 
GroupAccess::Open(
    const Hdf::Access& location, 
    const std::string& name, 
    const Hdf::PropertyListAccess& aapl)
{
    Close();
    m_Hid = H5Gopen(location, name.c_str(), aapl);
    if(Hdf::Access::IsValid(m_Hid) == false)
    {
        vdLogGlobalError("Failed to open group: '%s'", name.c_str());
        return false;
    }
    m_Name = name;
    return true;
}

std::string
GroupAccess::GetLocalName() const
{
    char local_name[VD_HDF5_MAX_NAME] = {0};
    if(Hdf::Access::IsValid(m_Hid) == false)
    {
        vdLogGlobalError("Invalid handle!  Failed to find local name for group: '%s'", m_Name.c_str());
        return std::string(local_name);    
    }
    H5Iget_name(m_Hid, local_name, VD_HDF5_MAX_NAME  );
    return std::string(local_name);    
}

size_t
GroupAccess::GetEntryCount() const
{
    hsize_t entry_count = 0;
    if(Hdf::Access::IsValid(m_Hid) == false)
    {
        vdLogGlobalError("Invalid handle!  Failed to find determine entry count for group: '%s'", m_Name.c_str());
        return 0;    
    }

    herr_t err = H5Gget_num_objs(m_Hid, &entry_count);
    if(err != 0)
    {
        vdLogGlobalError("Failed to determine group entry count for group '%s'", m_Name.c_str());
        return 0;
    }

    return entry_count;
}

std::string
GroupAccess::GetEntryName(size_t index) const
{
    char child_name[VD_HDF5_MAX_NAME] = {0};
    if(Hdf::Access::IsValid(m_Hid) == false)
    {
        vdLogGlobalError("Invalid handle!  Failed to find determine entry count for group: '%s'", m_Name.c_str());
        return std::string(child_name);
    }

    H5Gget_objname_by_idx(m_Hid, (hsize_t)index, child_name, (size_t)VD_HDF5_MAX_NAME );
    return std::string(child_name);
}
        
int
GroupAccess::GetEntryType(size_t index) const
{
    int obj_type = 0;
    if(Hdf::Access::IsValid(m_Hid) == false)
    {
        vdLogGlobalError("Invalid handle!  Failed to find determine entry type for group: '%s'", m_Name.c_str());
        return 0;
    }

    obj_type = H5Gget_objtype_by_idx(m_Hid, (size_t)index );
    return obj_type;
}

bool 
GroupAccess::Close()
{
    if(!IsValid()) return false;
    herr_t error = H5Gclose(m_Hid);
    if(error) vdLogGlobalError("Couldn't close group: '%s'", m_Name.c_str());
    Hdf::Access::Invalidate();
    return (error) ? false : true;
}

const std::string&
GroupAccess::GetName() const
{
    return m_Name;    
}

AccessClassId 
GroupAccess::GetAccessClassId() const
{
    return VD_ACCESS_CLASS_GROUP;
}

// ---------------------------------------------------------------------------------------------- //

SpaceAccess::SpaceAccess() 
    : Hdf::Access()
{
    // EMPTY
}

SpaceAccess::~SpaceAccess() 
{ 
    Close(); 
}

bool 
SpaceAccess::Open(
    const Hdf::DataSetAccess& location)
{
    Close();
    m_Hid = H5Dget_space(location);
    if(Hdf::Access::IsValid(m_Hid) == false)
    {
        vdLogGlobalError("Failed to open space access!");
        return false;
    }

    return true;
}

bool 
SpaceAccess::Open(
    const Hdf::AttribValueAccess& location)
{
    Close();
    m_Hid = H5Aget_space(location);
    if(Hdf::Access::IsValid(m_Hid) == false)
    {
        vdLogGlobalError("Failed to open space access!");
        return false;
    }

    return true;
}

bool 
SpaceAccess::Close()
{
    if(Hdf::Access::IsValid(m_Hid) == false) 
        return false;
    
    herr_t error = H5Sclose(m_Hid);
    if(error) vdLogGlobalError("Failed to close space access!");
    Hdf::Access::Invalidate();
    return (error) ? false : true;
}

size_t 
SpaceAccess::GetDimCount() const
{
    if(Hdf::Access::IsValid(m_Hid) == false) 
        return 0;

    size_t dims = H5Sget_simple_extent_ndims( m_Hid );
    return dims;
}

size_t 
SpaceAccess::GetExtents(
    std::vector<size_t>& extents) const
{
    std::vector<size_t> dummy;
    return GetExtentRange(extents, dummy);
}

size_t 
SpaceAccess::GetExtentRange(
    std::vector<size_t>& minimum, 
    std::vector<size_t>& maximum) const
{
    size_t dims = GetDimCount();
    if(dims < 1) return 0;

    hsize_t min_ext[dims];
    hsize_t max_ext[dims];
    H5Sget_simple_extent_dims(m_Hid, &min_ext[0], &max_ext[0] );
#if 0
    herr_t err = H5Sget_simple_extent_dims(m_Hid, &min_ext[0], &max_ext[0] );
    if(err != 0)
    {
        vdLogGlobalError("Failed to determine space extent!");
        return 0;
    }
#endif
    minimum.clear();
    maximum.clear();

    for(size_t n = 0; n < dims; n++)
    {
        minimum.push_back( min_ext[n] );
        maximum.push_back( max_ext[n] );
    }
    return dims;
}

bool
SpaceAccess::SelectHyperSlab(
    Storage::SpaceSelectOpId op,
    const Storage::HyperSlab& slab) const
{
    if(Hdf::Access::IsValid(m_Hid) == false)
    {
        vdLogGlobalError("Failed to select hyperslab! Invalid handle!");
        return false;
    }

    size_t ndim = slab.Region.size();

    hsize_t start[ndim];
    hsize_t stride[ndim];
    hsize_t count[ndim];
    hsize_t block[ndim];

    for(size_t n = 0; n < ndim; n++)
    {
        start[n] = slab.Region[n].Start;
        stride[n] = slab.Region[n].Stride;
        count[n] = slab.Region[n].Count;
        block[n] = slab.Region[n].Block;
    }

    H5S_seloper_t select = AdaptSelectOp::ToNative(op);
    herr_t err = H5Sselect_hyperslab(m_Hid, select, start, stride, count, block);
    if(err)
    {
        vdLogGlobalError("Failed to select hyperslab! Error encountered!");
        return false;
    }
    return true;
}

bool
SpaceAccess::SelectHyperSlab(
    const Storage::HyperSlab& slab) const
{
    return SelectHyperSlab(VD_SPACE_SELECT_SET, slab);
}

bool
SpaceAccess::SelectElements(
    Storage::SpaceSelectOpId op,
    const std::vector<size_t>& elements) const
{
    if(Hdf::Access::IsValid(m_Hid) == false)
    {
        vdLogGlobalError("Failed to select hyperslab! Invalid handle!");
        return false;
    }

    hsize_t count = elements.size();
    std::vector<hsize_t> indices(count);
    for(size_t n = 0; n < elements.size(); n++)
        indices[n] = elements[n];

    H5S_seloper_t select = AdaptSelectOp::ToNative(op);
    herr_t err = H5Sselect_elements(m_Hid, select, count, &indices[0]);
    if(err)
    {
        vdLogGlobalError("Failed to select elements! Error encountered!");
        return false;
    }
    return true;
}

bool
SpaceAccess::SelectElements(
    const std::vector<size_t>& elements) const
{
    return SelectElements(VD_SPACE_SELECT_SET, elements);
}

AccessClassId 
SpaceAccess::GetAccessClassId() const
{
    return VD_ACCESS_CLASS_ATTRIBUTE_LIST;
}

// ---------------------------------------------------------------------------------------------- //

AttribListAccess::AttribListAccess() 
    : Hdf::Access()
{
    // EMPTY
}

AttribListAccess::~AttribListAccess() 
{ 
    Close(); 
}

bool 
AttribListAccess::Open(
    const Hdf::Access& location, 
    const std::string& name)
{
    Close();
    m_Hid = H5Aopen(location, name.c_str(), H5P_DEFAULT);
    if(Hdf::Access::IsValid(m_Hid) == false)
    {
        vdLogGlobalError("Failed to open attribute list: '%s'", name.c_str());
        return false;
    }
    m_Name = name;
    return true;
}

bool 
AttribListAccess::Open(
    const Hdf::Access& location, 
    const std::string& name, 
    const Hdf::PropertyListAccess& aapl)
{
    Close();
    m_Hid = H5Aopen(location, name.c_str(), aapl);
    if(Hdf::Access::IsValid(m_Hid) == false)
    {
        vdLogGlobalError("Failed to open attribute list: '%s'", name.c_str());
        return false;
    }
    m_Name = name;
    return true;
}

bool 
AttribListAccess::Close()
{
    if(!IsValid()) return false;
    herr_t error = H5Aclose(m_Hid);
    if(error) vdLogGlobalError("Couldn't close attribute: '%s'", m_Name.c_str());
    Hdf::Access::Invalidate();
    m_Name = std::string();
    return (error) ? false : true;
}

const std::string&
AttribListAccess::GetName() const
{
    return m_Name;    
}

AccessClassId 
AttribListAccess::GetAccessClassId() const
{
    return VD_ACCESS_CLASS_ATTRIBUTE_LIST;
}

// ---------------------------------------------------------------------------------------------- //

AttribValueAccess::AttribValueAccess() 
    : Hdf::Access(), m_Index(VD_INVALID_INDEX) 
{
    // EMPTY
}

AttribValueAccess::~AttribValueAccess() 
{ 
    Close(); 
}

bool 
AttribValueAccess::Open(const Hdf::Access& location, uint32_t index)
{
    Close();
    m_Hid = H5Aopen_idx(location, index);
    if(Hdf::Access::IsValid(m_Hid) == false)
    {
        vdLogGlobalError("Failed to open attribute at index: '%d'", index);
        return false;
    }
    m_Index = index;
    return true;
}

bool 
AttribValueAccess::Close()
{
    if(!IsValid()) return false;
    herr_t error = H5Aclose(m_Hid);
    if(error) vdLogGlobalError("Failed to close attribute at index: '%d'", m_Index);
    Hdf::Access::Invalidate();
    m_Index = VD_INVALID_INDEX;
    return (error) ? false : true;
}

bool
AttribValueAccess::ReadData(
    void* ptr, size_t bytes) const
{
    TypeAccess attrib_type;
    attrib_type.Open(*this);

    SpaceAccess attrib_space;
    attrib_space.Open(*this);

    std::vector<size_t> extents;
    attrib_space.GetExtents(extents);

    size_t type_size = attrib_type.GetSizeInBytes();
    size_t byte_count = type_size;
    for(size_t m = 0; m < extents.size(); m++)
        byte_count *= extents[m];

    if(byte_count > bytes)
    {
        vdLogGlobalError("Insufficient bytes for buffer!  Failed to read attribute data at index: '%d'", m_Index);
        return false;
    }

    herr_t error = H5Aread(m_Hid, attrib_type, ptr);
    if(error)
    {
        vdLogGlobalError("Failed to read attribute data at index: '%d'", m_Index);
        return false;
    }
    return true;
}

uint32_t 
AttribValueAccess::GetListIndex() const
{ 
    return m_Index; 
}

std::string
AttribValueAccess::GetLocalName() const
{
    char attrib_name[VD_HDF5_MAX_NAME] = {0};
    if(!IsValid()) std::string(attrib_name);
    H5Aget_name(m_Hid, VD_HDF5_MAX_NAME, attrib_name );
    return std::string(attrib_name);
}

AccessClassId 
AttribValueAccess::GetAccessClassId() const
{
    return VD_ACCESS_CLASS_ATTRIBUTE_VALUE;
}

// ---------------------------------------------------------------------------------------------- //

DataSetAccess::DataSetAccess() 
    : Hdf::Access()
{
    // EMPTY
}

DataSetAccess::~DataSetAccess() 
{ 
    Close(); 
}

bool 
DataSetAccess::Open(
    const Hdf::Access& location, 
    const std::string& name)
{
    Close();
    m_Hid = H5Dopen(location, name.c_str(), H5P_DEFAULT);
    if(Hdf::Access::IsValid(m_Hid) == false)
    {
        vdLogGlobalError("Failed to open dataset: '%s'", name.c_str());
        return false;
    }
    m_Name = name;
    return true;
}

bool 
DataSetAccess::Open(
    const Hdf::Access& location, 
    const std::string& name, 
    const Hdf::PropertyListAccess& aapl)
{
    Close();
    m_Hid = H5Dopen(location, name.c_str(), aapl);
    if(Hdf::Access::IsValid(m_Hid) == false)
    {
        vdLogGlobalError("Failed to open dataset: '%s'", name.c_str());
        return false;
    }
    m_Name = name;
    return true;
}

bool 
DataSetAccess::Close()
{
    if(!IsValid()) return false;
    herr_t error = H5Dclose(m_Hid);
    if(error) vdLogGlobalError("Failed to close dataset: '%s'", m_Name.c_str());
    Hdf::Access::Invalidate();
    return (error) ? false : true;
}

size_t
DataSetAccess::GetStorageSize() const
{
    size_t bytes = 0;
    if(Hdf::Access::IsValid(m_Hid) == false)
    {
        vdLogGlobalError("Invalid Handle!  Failed to retrieve storage size for dataset: '%s'", m_Name.c_str());
        return 0;
    }
    bytes = H5Dget_storage_size(m_Hid);
    return bytes;
}

bool
DataSetAccess::ReadData(
    const SpaceAccess& dset_space,
    void* ptr, size_t bytes) const
{
    if(Hdf::Access::IsValid(m_Hid) == false)
    {
        vdLogGlobalError("Invalid Handle!  Failed to read data for dataset: '%s'", m_Name.c_str());
        return false;
    }

    TypeAccess mem_type;
    mem_type.Open(*this);
    hid_t mem_space = H5S_ALL;

    herr_t error = H5Dread(m_Hid, mem_type, mem_space, dset_space, H5P_DEFAULT, ptr);
    if(error)
    {
        vdLogGlobalError("Failed to read dataset data at index: '%s'", m_Name.c_str());
        return false;
    }
    return true;
}

bool 
DataSetAccess::ReadData(
    const Hdf::SpaceAccess& dset_space, 
    const Hdf::SpaceAccess& mem_space, 
    const Hdf::TypeAccess& mem_type, 
    void* ptr, size_t bytes) const
{
    if(Hdf::Access::IsValid(m_Hid) == false)
    {
        vdLogGlobalError("Invalid Handle!  Failed to read data for dataset: '%s'", m_Name.c_str());
        return false;
    }

    herr_t error = H5Dread(m_Hid, mem_type, mem_space, dset_space, H5P_DEFAULT, ptr);
    if(error)
    {
        vdLogGlobalError("Failed to read dataset data at index: '%s'", m_Name.c_str());
        return false;
    }
    return true;

}

const std::string&
DataSetAccess::GetName() const
{
    return m_Name;    
}

AccessClassId 
DataSetAccess::GetAccessClassId() const
{
    return VD_ACCESS_CLASS_DATASET;
}

// ============================================================================================== //

bool
ToPropertyGraph::Open(
    const std::string& filename,
    const std::string& options)
{
    Close();
    Hdf::FileAccess file_access;
    if(file_access.Open(filename, Storage::VD_ACCESS_MODE_READ_ONLY) == false)
        return false;

    m_InputFile = filename;
    return true;
}

bool
ToPropertyGraph::Close()
{
    m_InputFile = std::string();
    return true;
}

bool
ToPropertyGraph::ExportDataSet(
    const std::string& path, 
    const Storage::DataSpace region, 
    NodeType& dataset)
{
/*
    hid_t space_id 
    herr_t error = H5Sselect_hyperslab(
        hid_t space_id, 
        H5S_seloper_t op, 
        const hsize_t *start, 
        const hsize_t *stride, 
        const hsize_t *count, 
        const hsize_t *block );
*/
    return true;
}

/*
bool
ToPropertyGraph::ExportDataSet(
    const std::string& path, 
    const std::vector<Hdf::Range> region)
{
    AccessCursor child_cursor;
    child_cursor.Id = H5Dopen(group_cursor.Id, child_name, H5P_DEFAULT);
    child_cursor.Path = child_path;
    child_cursor.Name = child_name;
    child_cursor.Indent = group_cursor.Indent + "  ";

//    ReadDataSet(group_meta, child_cursor, region, dataset);
    H5Dclose(child_cursor.Id);    
}
*/

bool
ToPropertyGraph::ExportMetaData(
    GraphType& graph)
{
    Hdf::FileAccess file_access;
    if(file_access.Open(m_InputFile, Storage::VD_ACCESS_MODE_READ_ONLY) == false)
        return false;

    std::string group_path("/");
    Hdf::GroupAccess group_access;
    if(group_access.Open(file_access, group_path) == false)
        return false;

    graph.Clear();

    MetaInfo root_meta;
    root_meta.Set("Path", std::string(group_path) );
    root_meta.Set("Name", std::string(m_InputFile) );
    root_meta.Set("Root", true);
    root_meta.Set("Type", std::string("file") );
    NodeKey root_key = graph.AddNode(root_meta);

    AccessCursor group_cursor;
    group_cursor.Id = group_access;
    group_cursor.Name = group_path;
    ExportGroupMetaData(graph, root_key, group_access, group_cursor);
    return true;
}

void
ToPropertyGraph::ExportGroupMetaData(
    GraphType& graph, NodeKey& parent_key, 
    const GroupAccess& group_access,
    const AccessCursor& group_cursor)
{
    std::string group_name = group_access.GetLocalName();
    std::string group_path = group_cursor.Path; 
    std::string resolved_path = ((group_cursor.Path.length() > 0) ? group_cursor.Path : std::string("/"));

    MetaInfo group_meta;
    group_meta.Set("Path", resolved_path);
    group_meta.Set("Name", group_cursor.Name);
    group_meta.Set("Type", std::string("group"));
    NodeKey group_key = graph.AddNode(group_meta);
    
    MetaInfo edge_meta;
    edge_meta.Set("Path", resolved_path);
    graph.AddEdge(parent_key, group_key, edge_meta);

    AttribListAccess attrib_access;

    AccessCursor list_cursor;
    list_cursor.Id = group_cursor.Id;
    list_cursor.Path = group_path;
    list_cursor.Name = group_cursor.Name;
    list_cursor.Indent = group_cursor.Indent;
    ExportAttribList(graph, group_key, attrib_access, list_cursor);

    size_t child_count = group_access.GetEntryCount();
    for (size_t i = 0; (i < child_count); i++)
    {
        std::string child_name = group_access.GetEntryName(i);
        std::cout << group_cursor.Indent << "+ " << child_name;
        std::string child_path = group_path + std::string("/") + std::string(child_name);

        int obj_type = group_access.GetEntryType(i);
        switch(obj_type)
        {
        case H5G_LINK:
        {
            AccessCursor child_cursor;
            child_cursor.Id = group_cursor.Id;
            child_cursor.Index = i;
            child_cursor.Path = child_path;
            child_cursor.Name = child_name;
            child_cursor.Indent = group_cursor.Indent + "  ";

            ExportLink(graph, group_key, group_access, child_cursor);

            std::cout << " [ SYM_LINK ]" << std::endl;
            break;
        }
        case H5G_GROUP:
        {
            std::cout << " [ GROUP ] " << std::endl;

            GroupAccess child_group;
            child_group.Open(group_access, child_name);

            AccessCursor child_cursor;
            child_cursor.Id = child_group;
            child_cursor.Index = i;
            child_cursor.Path = child_path;
            child_cursor.Name = child_name;
            child_cursor.Indent = group_cursor.Indent + "  ";

            ExportGroupMetaData(graph, group_key, child_group, child_cursor);
            break;
        }
        case H5G_DATASET:
        {
            std::cout << " [ DATASET ]";

            DataSetAccess dset_access;
            dset_access.Open(group_access, child_name);

            AccessCursor child_cursor;
            child_cursor.Id = dset_access;
            child_cursor.Index = i;
            child_cursor.Path = child_path;
            child_cursor.Name = child_name;
            child_cursor.Indent = group_cursor.Indent + "  ";
            ExportDataSetMetaData(graph, group_key, dset_access, child_cursor);
            break;
        }
        case H5G_TYPE:
        {
            TypeAccess type_access;
            type_access.Open(group_access, child_name);

            AccessCursor child_cursor;
            child_cursor.Id = type_access; 
            child_cursor.Index = i;
            child_cursor.Path = child_path;
            child_cursor.Name = child_name;
            child_cursor.Indent = group_cursor.Indent + "  ";
            ExportType(graph, group_key, type_access, child_cursor);

            std::cout << std::endl;
            break;
        }
        default:
            break;
        };
    }
}

void
ToPropertyGraph::ExportDataSetMetaData(
    GraphType& graph, NodeKey& parent_key, 
    const DataSetAccess& dset_access, 
    const AccessCursor& dset_cursor)
{
    std::string dset_name = dset_access.GetName();
    std::string dset_path = dset_cursor.Path + std::string("/") + std::string(dset_cursor.Name);

    SpaceAccess dset_space;
    dset_space.Open(dset_access);
    int ndims = (int)dset_space.GetDimCount(); 
    size_t dset_storage_size = dset_access.GetStorageSize();
    
    TypeAccess dset_type;
    dset_type.Open(dset_access);

    MetaInfo dset_meta;
    dset_meta.Set("Path", std::string(dset_cursor.Path));
    dset_meta.Set("Name", std::string(dset_cursor.Name));
    dset_meta.Set("Type", std::string("dataset") );
    dset_meta.Set("Dimensions", ndims );
    dset_meta.Set("StorageSize", uint64_t(dset_storage_size) );
    NodeKey dset_key = graph.AddNode(dset_meta);

    TypeAccess type_access;
    type_access.Open(dset_access);

    AccessCursor type_cursor;
    type_cursor.Id = type_access;
    type_cursor.Path = dset_cursor.Path;
    type_cursor.Name = dset_cursor.Name;
    type_cursor.Indent = std::string(" ");
    ExportType(graph, dset_key, type_access, type_cursor);
    graph.GetNodeMeta(dset_key, dset_meta);

    size_t type_size = dset_type.GetSizeInBytes();
    size_t byte_size = type_size;

    if(ndims > 0)
    {
        std::vector<size_t> extents;
        dset_space.GetExtents(extents);
        PropertyValueList extent_prop;

        std::cout << " [";
        for(int m = 0; m < ndims; m++)
        {
            std::cout << " " << extents[m] << ((m < ndims-1) ? "," : " ");
            extent_prop.push_back(uint64_t(extents[m]));
            byte_size *= extents[m];
        }
        std::cout << "]";

        if(extent_prop.size() > 1)
            dset_meta.Set("Extents", extent_prop );
        else
            dset_meta.Set("Extents", extent_prop[0] );

        std::cout << std::endl;
    }
    dset_key = graph.SetNode(dset_key, dset_meta);

    // Storage::TypeClassId type_class = dset_type.GetTypeClassId();
    TypeAccess native_type = dset_type.GetNativeType(); 

// ----------------------------------------

    MetaInfo edge_meta;
    edge_meta.Set("Path", std::string(dset_cursor.Path));
    graph.AddEdge(parent_key, dset_key, edge_meta);

    AttribListAccess attrib_access;

    AccessCursor list_cursor;
    list_cursor.Id = dset_cursor.Id;
    list_cursor.Path = dset_cursor.Path;
    list_cursor.Name = dset_cursor.Name;
    list_cursor.Indent = dset_cursor.Indent + std::string("  ");
    ExportAttribList(graph, dset_key, attrib_access, list_cursor);

    // ... read data with H5Dread

}

void
ToPropertyGraph::ExportType(
    GraphType& graph, NodeKey& parent_key, 
    const TypeAccess& type_access, 
    const AccessCursor& type_cursor)
{
    MetaInfo node_meta;
    graph.GetNodeMeta(parent_key, node_meta);

    H5T_class_t t_class;
    t_class = H5Tget_class(type_cursor.Id);
    if(t_class < 0)
    {
        node_meta.Set("DataType", std::string("invalid") );
    }
    else
    {
        if(t_class == H5T_INTEGER)
        {
            std::cout << " [ H5T_INTEGER ]";
            node_meta.Set("DataType", std::string("int") );
        }
        else if(t_class == H5T_FLOAT)
        {
            std::cout << " [ H5T_FLOAT ]";
            node_meta.Set("DataType", std::string("float") );
        }
        else if(t_class == H5T_STRING)
        {
            std::cout << " [ H5T_STRING ]";
            node_meta.Set("DataType", std::string("string") );
        }
        else if(t_class == H5T_BITFIELD)
        {
            std::cout << " [ H5T_BITFIELD ]";
            node_meta.Set("DataType", std::string("bitfield") );
        }
        else if(t_class == H5T_OPAQUE)
        {
            std::cout << " [ H5T_OPAQUE ]";
            node_meta.Set("DataType", std::string("opaque") );
        }
        else if(t_class == H5T_COMPOUND)
        {
            std::cout << " [ H5T_COMPOUND ]";
            node_meta.Set("DataType", std::string("compound") );
        }
        else if(t_class == H5T_ARRAY)
        {
            std::cout << " [ H5T_ARRAY ]";
            node_meta.Set("DataType", std::string("array") );
        }
        else if(t_class == H5T_ENUM)
        {
            std::cout << " [ H5T_ENUM ]";
            node_meta.Set("DataType", std::string("enum") );
        }
        else
        {
            node_meta.Set("DataType", std::string("unknown") );
        }
    }

    graph.SetNode(parent_key, node_meta);
}


void
ToPropertyGraph::ExportLink(
    GraphType& graph, NodeKey& parent_key, 
    const GroupAccess& group_access, 
    const AccessCursor& link_cursor)
{
    char target[VD_HDF5_MAX_NAME] = {0};
    H5Gget_linkval(link_cursor.Id, link_cursor.Name.c_str(), VD_HDF5_MAX_NAME, target  ) ;

    std::cout << link_cursor.Name << " -> " << target;

    MetaInfo node_meta;
    node_meta.Set("Path", std::string(link_cursor.Path) );
    node_meta.Set("Name", std::string(link_cursor.Name) );
    node_meta.Set("Type", std::string("link") );
    node_meta.Set("Target", std::string(target) );
    NodeKey node_key = graph.AddNode(node_meta);

    MetaInfo edge_meta;
    edge_meta.Set("Path", std::string(link_cursor.Path));
    graph.AddEdge(parent_key, node_key, edge_meta);
}

void
ToPropertyGraph::ExportAttribList(
    GraphType& graph, NodeKey& parent_key, 
    const AttribListAccess& attrib_list, 
    const AccessCursor& list_cursor) 
{
    int na = H5Aget_num_attrs(list_cursor.Id);
    for (int i = 0; i < na; i++)
    {
        AccessCursor attrib_cursor;
        attrib_cursor.Id = list_cursor.Id;
        attrib_cursor.Index = i;
        attrib_cursor.Path = list_cursor.Path;
        attrib_cursor.Name = list_cursor.Name;
        attrib_cursor.Indent = list_cursor.Indent;

        AttribValueAccess attrib_access;
        ExportAttribValue(graph, parent_key, attrib_access, attrib_cursor);

        std::cout << std::endl;
    }
}

void
ToPropertyGraph::ExportAttribValue(
    GraphType& graph, NodeKey& parent_key, 
    const AttribValueAccess& attrib_access,
    const AccessCursor& attrib_cursor) 
{
    AttribValueAccess attrib_value;
    attrib_value.Open(attrib_cursor.Id, attrib_cursor.Index);
    std::string attrib_name = attrib_value.GetLocalName();

    std::string attrib_path = attrib_cursor.Path; // + std::string("/") + std::string(attrib_base);
    std::cout << attrib_cursor.Indent << "- " << attrib_name ;

    TypeAccess attrib_type;
    attrib_type.Open(attrib_value);

    SpaceAccess attrib_space;
    attrib_space.Open(attrib_value);
    int ndims = (int)attrib_space.GetDimCount(); 

    MetaInfo attrib_meta;
    attrib_meta.Set("Path", attrib_path );
    attrib_meta.Set("Name", std::string(attrib_name) );
    attrib_meta.Set("Type", std::string("attribute") );
    attrib_meta.Set("Dimensions", ndims );
    NodeKey attrib_key = graph.AddNode(attrib_meta);

    TypeAccess type_access;
    
    AccessCursor type_cursor;
    type_cursor.Id = attrib_type;
    type_cursor.Path = attrib_cursor.Path;
    type_cursor.Name = attrib_cursor.Name;
    type_cursor.Indent = std::string(" ");
    ExportType(graph, attrib_key, type_access, type_cursor);
    graph.GetNodeMeta(attrib_key, attrib_meta);

    size_t type_size = attrib_type.GetSizeInBytes(); 
    size_t byte_size = type_size;
    size_t entry_count = 1;
    if(ndims > 0)
    {
        std::vector<size_t> extents;
        attrib_space.GetExtents(extents);
        PropertyValueList extent_prop;

        std::cout << " [";
        for(int m = 0; m < ndims; m++)
        {
            std::cout << " " << extents[m] << ((m < ndims-1) ? "," : " ");
            extent_prop.push_back(uint64_t(extents[m]));
            byte_size *= extents[m];
            entry_count *= extents[m];
        }
        std::cout << "]";

        if(extent_prop.size() > 1)
            attrib_meta.Set("Extents", extent_prop );
        else
            attrib_meta.Set("Extents", extent_prop[0] );
    }

    H5T_class_t type_class = H5Tget_class(attrib_type);
    TypeAccess native_type = attrib_type.GetNativeType(); 
    PropertyValueList attrib_data;

    std::vector<int8_t> buffer(byte_size);
    void* raw = &buffer[0];

    bool success = attrib_value.ReadData(raw, byte_size);
    if(success)
    {
        PropertyValueList attrib_value;
        if(H5Tequal(attrib_type, H5T_NATIVE_SCHAR))
        {
            int8_t* i = (int8_t*)raw;
            for(size_t n = 0; n < (byte_size / sizeof(int8_t)); n++)
                attrib_value.push_back(i[n]);
        }
        else if(H5Tequal(attrib_type, H5T_NATIVE_SHORT))
        {
            int16_t* i = (int16_t*)raw;
            for(size_t n = 0; n < (byte_size / sizeof(int16_t)); n++)
                attrib_value.push_back(i[n]);
        }
        else if(H5Tequal(attrib_type, H5T_NATIVE_INT))
        {
            int32_t* i = (int32_t*)raw;
            for(size_t n = 0; n < (byte_size / sizeof(int32_t)); n++)
                attrib_value.push_back(i[n]);
            }
        if(H5Tequal(attrib_type, H5T_NATIVE_UCHAR))
        {
            uint8_t* i = (uint8_t*)raw;
            for(size_t n = 0; n < (byte_size / sizeof(uint8_t)); n++)
                attrib_value.push_back(i[n]);
        }
        else if(H5Tequal(attrib_type, H5T_NATIVE_USHORT))
        {
            uint16_t* i = (uint16_t*)raw;
            for(size_t n = 0; n < (byte_size / sizeof(uint16_t)); n++)
                attrib_value.push_back(i[n]);
        }
        else if(H5Tequal(attrib_type, H5T_NATIVE_UINT))
        {
            uint32_t* i = (uint32_t*)raw;
            for(size_t n = 0; n < (byte_size / sizeof(uint32_t)); n++)
                attrib_value.push_back(i[n]);
        }
        else if(H5Tequal(attrib_type, H5T_NATIVE_FLOAT))
        {
            float* f = (float*)raw;
            for(size_t n = 0; n < (byte_size / sizeof(float)); n++)
                attrib_value.push_back(f[n]);
        }
        else if(H5Tequal(attrib_type, H5T_NATIVE_DOUBLE))
        {
            double* d = (double*)raw;
            for(size_t n = 0; n < (byte_size / sizeof(double)); n++)
                attrib_value.push_back(d[n]);
        }
        else if(H5Tequal(attrib_type, H5T_C_S1))
        {
            char* c = (char*)raw;
            std::string str(c);
            attrib_value.push_back(str);
        }
        else if(type_class == H5T_STRING)
        {
            char* c = (char*)raw;
            std::string str(c);
            attrib_value.push_back(str);
        }

        if(attrib_value.size() > 1)
            attrib_meta.Set("Value", attrib_value);
        else if(attrib_value.size() > 0)
            attrib_meta.Set("Value", attrib_value[0]);
    }
    attrib_key = graph.SetNode(attrib_key, attrib_meta);

    MetaInfo edge_meta;
    edge_meta.Set("Path", std::string(attrib_path) );
    graph.AddEdge(parent_key, attrib_key, edge_meta);
}

// ============================================================================================== //

} // end namespace: Hdf

// ============================================================================================== //

} // end namespace: Storage

// ----------------------------------------------------------------------------------------------- //
