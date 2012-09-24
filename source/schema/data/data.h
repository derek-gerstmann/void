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

#ifndef VD_SCHEMA_DATA_INCLUDED
#define VD_SCHEMA_DATA_INCLUDED

// ============================================================================================== //

#include "schema/schema.h"

// ============================================================================================== //

VD_SCHEMA_NAMESPACE_BEGIN();

// ============================================================================================== //

namespace Data {

// ---------------------------------------------------------------------------------------------- //

enum ModelTypeId
{
    VD_DATA_MODEL_TYPE_INVALID,
    VD_DATA_MODEL_TYPE_META,
    VD_DATA_MODEL_TYPE_VALUE,
    VD_DATA_MODEL_TYPE_PROPERTY,
    VD_DATA_MODEL_TYPE_EDGE,
    VD_DATA_MODEL_TYPE_NODE,
    VD_DATA_MODEL_TYPE_GRAPH,
    VD_DATA_MODEL_TYPE_LAST_ID
};

// ---------------------------------------------------------------------------------------------- //

struct MetaKey
{
    vd::u64     Id;
    ModelTypeId Type;

    MetaKey();
    static bool IsValid(MetaKey mk);
};

// ---------------------------------------------------------------------------------------------- //

typedef std::vector<MetaKey> MetaKeyList;

// ============================================================================================== //

} // end namespace: Data

// ============================================================================================== //

VD_SCHEMA_NAMESPACE_END();

// ============================================================================================== //

#endif  // VD_SCHEMA_DATA_INCLUDED
