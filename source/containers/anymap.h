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

#ifndef VD_CONTAINERS_ANYMAP_INCLUDED
#define VD_CONTAINERS_ANYMAP_INCLUDED

// ============================================================================================== //

#include "containers/namespace.h"
#include "core/core.h"
#include "core/allocators.h"

// ============================================================================================== //

#include <boost/any.hpp>
#include <boost/unordered_map.hpp>

// ============================================================================================== //

VD_CONTAINERS_NAMESPACE_BEGIN();

// ============================================================================================== //

template< class KeyType, class HashType >
class AnyMap
{
public:

	typedef boost::unordered_map<KeyType, boost::any, HashType, std::equal_to<KeyType>, 
								 Core::AlignedAllocator< std::pair<const KeyType, boost::any> > > ValueMapType;

public:

	template < typename T > 
	const T& Get(const KeyType& key) 
	{ 
		return boost::any_cast<const T&>(m_Values[key]); 
	} 

	template < typename T > 
	bool Set(const KeyType& key, const T& value) 
	{
		m_Values[key] = value; 
		return true;
	} 

	template < typename T > 
	boost::any& Set(const KeyType& key) 
	{ 
		if(!exists(key))
		{
			m_Values[key] = boost::any();
		}	
		return m_Values[key];
	} 

	template < typename T > 
	const T& operator()(const KeyType& key) const
	{ 
		return m_Values[key];
	} 

	template < typename T > 
	T& operator()(const KeyType& key)
	{ 
		return m_Values[key];
	} 

    template <class T>
    const T& operator[](const KeyType& key) const
    {
		return boost::any_cast<const T&>(m_Values[key]); 
    }

    size_t erase(const KeyType& key)
    {
        return m_Values.erase(key);
    }

    void clear()
    {
        m_Values.clear();
    }

    size_t size() const
    {
        return m_Values.size();
    }

    bool empty() const
    {
        return m_Values.empty();
    }

	bool exists(const KeyType& key) const
	{
		return m_Values.find(key) != m_Values.end();
	}


private:
	ValueMapType m_Values;
};

// ============================================================================================== //

VD_CONTAINERS_NAMESPACE_END();

// ============================================================================================== //

#endif	// VD_CONTAINERS_ANYMAP_INCLUDED
