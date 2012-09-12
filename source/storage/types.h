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

#ifndef VD_STORAGE_TYPES_INCLUDED
#define VD_STORAGE_TYPES_INCLUDED

// ============================================================================================== //

#include "core/object.h"

// ============================================================================================== //

VD_STORAGE_NAMESPACE_BEGIN();

// ============================================================================================== //

VD_USING(Core, Object);

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

VD_STORAGE_NAMESPACE_END();

// ============================================================================================== //

#endif // VD_STORAGE_TYPES_INCLUDED
