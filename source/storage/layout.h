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

#ifndef VD_STORAGE_LAYOUT_INCLUDED
#define VD_STORAGE_LAYOUT_INCLUDED

// ============================================================================================== //

#include "storage/storage.h"

// ============================================================================================== //

VD_STORAGE_NAMESPACE_BEGIN();

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

VD_STORAGE_NAMESPACE_END();

// ============================================================================================== //

#endif // VD_STORAGE_LAYOUT_INCLUDED
