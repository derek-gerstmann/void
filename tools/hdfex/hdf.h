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

#include <hdf5.h>
#include "std.h"
#include "dmpg.h"
#include "neo4j.h"

// ============================================================================================== //

#ifndef VD_DATA_FORMAT_HDF_INCLUDED
#define VD_DATA_FORMAT_HDF_INCLUDED

// ============================================================================================== //

#define VD_HDF5_MAX_NAME (1024)

// ============================================================================================== //

namespace Storage {

// ============================================================================================== //

enum StdTypeId 
{
    VD_STD_TYPE_INVALID,
    VD_STD_TYPE_I8,
    VD_STD_TYPE_I16,
    VD_STD_TYPE_I32,
    VD_STD_TYPE_I64,
    VD_STD_TYPE_U8,
    VD_STD_TYPE_U16,
    VD_STD_TYPE_U32,
    VD_STD_TYPE_U64,
    VD_STD_TYPE_F32,
    VD_STD_TYPE_F64,
    VD_STD_TYPE_UNKNOWN,
    VD_STD_TYPE_LAST_ID
};

enum TypeClassId
{
    VD_TYPE_CLASS_INVALID,
    VD_TYPE_CLASS_INTEGER,
    VD_TYPE_CLASS_FLOAT,
    VD_TYPE_CLASS_STRING,
    VD_TYPE_CLASS_BITFIELD,
    VD_TYPE_CLASS_OPAQUE,
    VD_TYPE_CLASS_COMPOUND,
    VD_TYPE_CLASS_ARRAY,
    VD_TYPE_CLASS_ENUM,
    VD_TYPE_CLASS_UNKNOWN,
    VD_TYPE_CLASS_LAST_ID
};

enum AccessModeId 
{
    VD_ACCESS_MODE_INVALID,
    VD_ACCESS_MODE_READ_ONLY,
    VD_ACCESS_MODE_WRITE_ONLY,
    VD_ACCESS_MODE_READ_WRITE,
    VD_ACCESS_MODE_UNKNOWN,
    VD_ACCESS_MODE_LAST_ID
};

enum AccessClassId 
{
    VD_ACCESS_CLASS_INVALID,
    VD_ACCESS_CLASS_ANY,
    VD_ACCESS_CLASS_FILE,
    VD_ACCESS_CLASS_GROUP,
    VD_ACCESS_CLASS_TYPE,
    VD_ACCESS_CLASS_SPACE,
    VD_ACCESS_CLASS_DATASET,
    VD_ACCESS_CLASS_PROPERTY_LIST,
    VD_ACCESS_CLASS_ATTRIBUTE_LIST,
    VD_ACCESS_CLASS_ATTRIBUTE_VALUE,
    VD_ACCESS_CLASS_LAST_ID
};

enum PropertyListTypeId 
{
    VD_PROPERTY_LIST_INVALID,
    VD_PROPERTY_LIST_CREATE,
    VD_PROPERTY_LIST_ACCESS,
    VD_PROPERTY_LIST_UNKNOWN,
    VD_PROPERTY_LIST_LAST_ID
};

// ============================================================================================== //

struct DataLayout
{
    size_t                          Offset;
    size_t                          Stride;
    size_t                          Extent;
    size_t                          Block;

    DataLayout() :
     Offset(0), Stride(0), Extent(0), Block(0) { }
};

struct DataSpace
{
    size_t                          Dimensions;
    std::vector<size_t>             Extents;
    std::vector<DataLayout>         Region;

    DataSpace() :
     Dimensions(0) { }
};

// ============================================================================================== //

namespace Hdf {

// ============================================================================================== //

template <typename T>
struct AdaptStandardType 
{  
    static hid_t ToNativeTypeId() 
    {
        return 0;
    }

    static size_t SizeOf()
    {
        return sizeof(T);
    }

    static StdTypeId ToStdTypeId() 
    {
        return VD_STD_TYPE_INVALID;
    }
};

// ============================================================================================== //

template <> inline hid_t AdaptStandardType<bool>::ToNativeTypeId()     { return H5T_NATIVE_CHAR;    }
template <> inline hid_t AdaptStandardType<int8_t>::ToNativeTypeId()   { return H5T_NATIVE_CHAR;    }
template <> inline hid_t AdaptStandardType<int16_t>::ToNativeTypeId()  { return H5T_NATIVE_SHORT;   }
template <> inline hid_t AdaptStandardType<int32_t>::ToNativeTypeId()  { return H5T_NATIVE_INT;     }
template <> inline hid_t AdaptStandardType<int64_t>::ToNativeTypeId()  { return H5T_NATIVE_LONG;    }
template <> inline hid_t AdaptStandardType<uint8_t>::ToNativeTypeId()  { return H5T_NATIVE_UCHAR;   }
template <> inline hid_t AdaptStandardType<uint16_t>::ToNativeTypeId() { return H5T_NATIVE_USHORT;  }
template <> inline hid_t AdaptStandardType<uint32_t>::ToNativeTypeId() { return H5T_NATIVE_UINT;    }
template <> inline hid_t AdaptStandardType<uint64_t>::ToNativeTypeId() { return H5T_NATIVE_ULONG;   }
template <> inline hid_t AdaptStandardType<float>::ToNativeTypeId()    { return H5T_NATIVE_FLOAT;   }
template <> inline hid_t AdaptStandardType<double>::ToNativeTypeId()   { return H5T_NATIVE_DOUBLE;  }

template <> inline StdTypeId AdaptStandardType<bool>::ToStdTypeId()     { return VD_STD_TYPE_I8;     }
template <> inline StdTypeId AdaptStandardType<int8_t>::ToStdTypeId()   { return VD_STD_TYPE_I8;     }
template <> inline StdTypeId AdaptStandardType<int16_t>::ToStdTypeId()  { return VD_STD_TYPE_I16;    }
template <> inline StdTypeId AdaptStandardType<int32_t>::ToStdTypeId()  { return VD_STD_TYPE_I32;    }
template <> inline StdTypeId AdaptStandardType<int64_t>::ToStdTypeId()  { return VD_STD_TYPE_I64;    }
template <> inline StdTypeId AdaptStandardType<uint8_t>::ToStdTypeId()  { return VD_STD_TYPE_U8;     }
template <> inline StdTypeId AdaptStandardType<uint16_t>::ToStdTypeId() { return VD_STD_TYPE_U16;    }
template <> inline StdTypeId AdaptStandardType<uint32_t>::ToStdTypeId() { return VD_STD_TYPE_U32;    }
template <> inline StdTypeId AdaptStandardType<uint64_t>::ToStdTypeId() { return VD_STD_TYPE_U64;    }
template <> inline StdTypeId AdaptStandardType<float>::ToStdTypeId()    { return VD_STD_TYPE_F32;    }
template <> inline StdTypeId AdaptStandardType<double>::ToStdTypeId()   { return VD_STD_TYPE_F64;    }

// ============================================================================================== //

struct AdaptNativeType 
{  
    static StdTypeId ToStdTypeId(hid_t v) 
    {
        if(v == 0)                      { return VD_STD_TYPE_INVALID; }
        else if(v == H5T_NATIVE_CHAR)   { return VD_STD_TYPE_I8; }
        else if(v == H5T_NATIVE_SHORT)  { return VD_STD_TYPE_I16; }
        else if(v == H5T_NATIVE_INT)    { return VD_STD_TYPE_I32; }
        else if(v == H5T_NATIVE_LONG)   { return VD_STD_TYPE_I64; }
        else if(v == H5T_NATIVE_UCHAR)  { return VD_STD_TYPE_U8; }
        else if(v == H5T_NATIVE_USHORT) { return VD_STD_TYPE_U16; }
        else if(v == H5T_NATIVE_UINT)   { return VD_STD_TYPE_U32; }
        else if(v == H5T_NATIVE_ULONG)  { return VD_STD_TYPE_U64; }
        else if(v == H5T_NATIVE_FLOAT)  { return VD_STD_TYPE_F32; }
        else if(v == H5T_NATIVE_DOUBLE) { return VD_STD_TYPE_F64; }
        else                            { return VD_STD_TYPE_INVALID; }
        return VD_STD_TYPE_INVALID;
    }

    static size_t SizeOf(hid_t v)
    {
        return H5Tget_size(v);
    }
};

// ----------------------------------------------------------------------------------------------- //

struct AdaptFileAccessMode 
{  
    static uint32_t ToAccessFlag(AccessModeId access) 
    {
        switch(access)
        {
            case VD_ACCESS_MODE_READ_ONLY:   { return H5F_ACC_RDONLY;   }
            case VD_ACCESS_MODE_WRITE_ONLY:  { return H5F_ACC_RDWR;     }
            case VD_ACCESS_MODE_READ_WRITE:  { return H5F_ACC_RDWR;     }
            default:                         { return VD_INVALID_INDEX; }
        };
        return VD_INVALID_INDEX;
    }

};

// ============================================================================================== //

std::string Basename(const std::string& name);

// ---------------------------------------------------------------------------------------------- //

class Access;
class InvalidAccess;
class FileAccess;
class GroupAccess;
class TypeAccess;
class SpaceAccess;
class DataSetAccess;
class AttribListAccess;
class AttribValueAccess;
class PropertyListAccess;

// ----------------------------------------------------------------------------------------------- //

class Access
{
public:
    Access();
    Access(hid_t h);
    virtual ~Access();

    bool IsValid() const;
    static bool IsValid(hid_t h);

    const operator hid_t() const;
    virtual AccessClassId GetAccessClassId() const;

protected:
    void Invalidate();

protected:
    hid_t m_Hid;
};

// ---------------------------------------------------------------------------------------------- //

class InvalidAccess : public Hdf::Access
{
public:
    InvalidAccess();
    virtual ~InvalidAccess();
    virtual AccessClassId GetAccessClassId() const;
};

// ---------------------------------------------------------------------------------------------- //

class PropertyListAccess : public Hdf::Access
{
public:
    PropertyListAccess();
    virtual ~PropertyListAccess();
    
    bool Open(const FileAccess& access, PropertyListTypeId type );
    bool Open(const GroupAccess& access, PropertyListTypeId type );
    bool Open(const TypeAccess& access, PropertyListTypeId type );
    bool Open(const DataSetAccess& access, PropertyListTypeId type );
    bool Open(const AttribListAccess& access, PropertyListTypeId type );

    bool Close();

    virtual AccessClassId GetAccessClassId() const;
};

// ---------------------------------------------------------------------------------------------- //

class TypeAccess : public Hdf::Access
{
public:
    TypeAccess();
    virtual ~TypeAccess();

    bool Open(const Hdf::DataSetAccess& access);
    bool Open(const Hdf::AttribValueAccess& access);
    bool Open(const Hdf::Access& location, const std::string& name);
    bool Open(const Hdf::Access& location, const std::string& name, const PropertyListAccess& aapl);
    bool Close();

    const std::string& GetName() const;
    Hdf::TypeAccess GetNativeType() const;
    size_t GetSizeInBytes() const;
    TypeClassId GetTypeClassId() const;
    virtual AccessClassId GetAccessClassId() const;

protected:
    std::string m_Name;
};

// ---------------------------------------------------------------------------------------------- //

class FileAccess : public Hdf::Access
{
public:
    FileAccess();
    virtual ~FileAccess();

    bool Open(const std::string& filename, AccessModeId mode);
    bool Close();

    const std::string& GetFileName() const;
    virtual AccessClassId GetAccessClassId() const;

private:

    std::string m_FileName;
};

// ---------------------------------------------------------------------------------------------- //

class SpaceAccess : public Hdf::Access
{
public:
    SpaceAccess();
    virtual ~SpaceAccess();

    bool Open(const Hdf::DataSetAccess& access);
    bool Open(const Hdf::AttribValueAccess& access);
    bool Close();

    size_t GetDimCount() const;
    size_t GetExtents(std::vector<size_t>& extents) const;
    size_t GetExtentRange(std::vector<size_t>& min_extents, std::vector<size_t>& max_extents) const;

    virtual AccessClassId GetAccessClassId() const;
};

// ---------------------------------------------------------------------------------------------- //

class DataSetAccess : public Hdf::Access
{
public:
    DataSetAccess();
    virtual ~DataSetAccess();

    bool Open(const Hdf::Access& location, const std::string& name);
    bool Open(const Hdf::Access& location, const std::string& name, const PropertyListAccess& aapl);
    bool Close();

    const std::string& GetName() const;
    virtual AccessClassId GetAccessClassId() const;

private:

    std::string m_Name;
};

// ---------------------------------------------------------------------------------------------- //

class GroupAccess : public Hdf::Access
{
public:

    GroupAccess();
    virtual ~GroupAccess();

    bool Open(const Hdf::Access& location, const std::string& name);
    bool Open(const Hdf::Access& location, const std::string& name, const PropertyListAccess& aapl);
    bool Close();

    std::string GetLocalName() const;

    size_t GetEntryCount() const;
    std::string GetEntryName(size_t index) const;    
    int GetEntryType(size_t index) const;

    const std::string& GetName() const;
    virtual AccessClassId GetAccessClassId() const;

private:

    std::string m_Name;
};

// ---------------------------------------------------------------------------------------------- //

class AttribListAccess : public Hdf::Access
{
public:

    AttribListAccess();
    virtual ~AttribListAccess();

    bool Open(const Hdf::Access& location);
    bool Open(const Hdf::Access& location, const std::string& name);
    bool Open(const Hdf::Access& location, const std::string& name, const PropertyListAccess& aapl);
    bool Close();

    const std::string& GetName() const;
    virtual AccessClassId GetAccessClassId() const;

private:

    std::string m_Name;
};

// ---------------------------------------------------------------------------------------------- //

class AttribValueAccess : public Hdf::Access
{
public:
    
    AttribValueAccess();
    virtual ~AttribValueAccess();

    bool Open(const Hdf::Access& location, uint32_t index);
    bool Close();

    uint32_t GetListIndex() const;
    std::string GetLocalName() const;
    virtual AccessClassId GetAccessClassId() const;

private:

    uint32_t m_Index;

};

// ============================================================================================== //

class ToPropertyGraph
{
public:
    typedef Data::Model::Meta                               MetaInfo;
    typedef Data::Model::Graph::Connected                   GraphType;
    typedef Data::Model::Graph::Node                        NodeType;
    typedef Data::Model::Graph::NodeKey                     NodeKey;
    typedef Data::Model::Graph::Edge                        EdgeType;
    typedef Data::Model::Graph::EdgeKey                     EdgeKey;
    typedef Data::Model::Property                           PropertyType;

    typedef std::vector< PropertyType::ValueType >          PropertyValueList;
    typedef std::vector< PropertyType >                     PropertyList;
    typedef std::vector< EdgeType >                         EdgeList;
    typedef std::vector< NodeType >                         NodeList;

    struct Cursor
    {
        Hdf::Access Id; 
        size_t      Index;
        std::string Path;
        std::string Name; 
        std::string Indent;
    };

public:
    ToPropertyGraph() {}
    ~ToPropertyGraph() {}

    bool Open(const std::string& filename, const std::string& options = "");
    bool Close();

    bool ExportMetaData(GraphType& graph);
    bool ExportDataSet(const std::string& path, const Storage::DataSpace region, NodeType& dataset);

protected:

    void ExportGroupMetaData(
        GraphType& graph, NodeKey& node_key, 
        const GroupAccess& group_access,
        const Cursor& cursor);

    void ExportDataSetMetaData(
        GraphType& graph, NodeKey& node_key, 
        const DataSetAccess& dset_access,
        const Cursor& cursor);

    void ExportAttribList(
        GraphType& graph, NodeKey& node_key, 
        const AttribListAccess& attrib_access,
        const Cursor& cursor);

    void ExportAttribValue(
        GraphType& graph, NodeKey& node_key, 
        const AttribValueAccess& attrib_access,
        const Cursor& cursor);

    void ExportType(
        GraphType& graph, NodeKey& node_key, 
        const TypeAccess& type_access,
        const Cursor& cursor);

    void ExportLink(
        GraphType& graph, NodeKey& node_key, 
        const GroupAccess& group_access,
        const Cursor& cursor);

private:
    std::vector<uint32_t>   m_DataSets;
    std::string             m_InputFile;
};

// ============================================================================================== //

} // end namespace: Hdf
} // end namespace: Storage

// ============================================================================================== //

#endif // VD_DATA_FORMAT_HDF_INCLUDED
