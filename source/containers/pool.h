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

#ifndef VD_CONTAINERS_POOL_INCLUDED
#define VD_CONTAINERS_POOL_INCLUDED

// ============================================================================================== //

#include "containers/containers.h"
#include "core/core.h"
#include "core/memory.h"

// ============================================================================================== //

VD_CONTAINERS_NAMESPACE_BEGIN();

// ============================================================================================== //

class Pool
{
public:

    Pool();
    ~Pool();

    bool Reserve(size_t bytes = (4 * 1024 * 1024));
    void Destroy();

    void* Allocate(size_t size);
    bool Deallocate(void* ptr);
    bool IsFull(size_t size) const;
    size_t GetCapacity() const { return m_Capacity; }

protected:

	typedef OrderedMap<void*, size_t>::type RegionMap;
	void* FindNextRegion(size_t size) const;

private:

    void* m_Buffer;
    size_t m_Capacity;
    RegionMap m_Regions;

	VD_DISABLE_COPY_CONSTRUCTORS(Pool);
};

// ============================================================================================== //

VD_CONTAINERS_NAMESPACE_END();

// ============================================================================================== //

#endif
