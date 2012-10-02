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

#include "core/pool.h"
#include "core/system.h"
#include "core/collections.h"

// ============================================================================================== //

VD_CORE_NAMESPACE_BEGIN();

// ============================================================================================== //

bool
Pool::Reserve(size_t size)
{
    Destroy();

    m_Buffer = VD_NEW_ARRAY(char, size);
    if(m_Buffer == NULL)
        return false;

    m_Capacity = size;
    return true;
}

Pool::~Pool()
{
    Destroy();
}

void Pool::Destroy()
{
    m_Regions.clear();
    m_Capacity = 0;
    VD_DELETE_ARRAY((char*)m_Buffer);
    m_Buffer = NULL;
}

void*
Pool::Allocate(
    size_t size)
{
    void* ptr = FindNextRegion(size);
    if(ptr != NULL)
    {
        m_Regions.insert(std::pair<void*, size_t>(ptr, size));
        return ptr;
    }

    return NULL;
}

bool
Pool::Deallocate(
    void* ptr)
{
    RegionMap::iterator it = m_Regions.find(ptr);
    if(it == m_Regions.end())
        return false;

    m_Regions.erase(it);
    return true;
}

bool
Pool::IsFull(size_t size) const
{
    return FindNextRegion(size) != NULL;
}

void*
Pool::FindNextRegion(size_t size) const
{
    RegionMap::const_iterator it = m_Regions.begin();
    RegionMap::const_iterator end = m_Regions.end();
    RegionMap::const_iterator last = it;

    void* ptr = NULL;
    size_t used = m_Regions.size();

    if(used == 0)
        return m_Buffer;

    if(it == end)
    {
        it--;
        ptr = (void*)(((size_t)it->first) + it->second + 1);

        if((size_t)ptr + size > (size_t)m_Buffer + m_Capacity)
            ptr = NULL;
    }
    else
    {
        for(it++; it != end; it++, last++)
        {
            if(((size_t)it->first) - ((size_t)last->first + last->second) > size)
            {
                ptr = (void*)(((size_t)last->first) + last->second + 1);
                break;
            }
        }
    }

    return ptr;
}

// ============================================================================================== //

VD_CORE_NAMESPACE_END();

// ============================================================================================== //
