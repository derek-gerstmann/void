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

#ifndef VD_CONTAINERS_PROPERTY_SET_INCLUDED
#define VD_CONTAINERS_PROPERTY_SET_INCLUDED

// ============================================================================================== //

#include "containers/containers.h"

// ============================================================================================== //

VD_CONTAINERS_NAMESPACE_BEGIN();

// ============================================================================================== //

/*

class PropertySetBase
{
public:
	template<typename T>
	struct Identity { typedef T type; };


	template< typename T >
	const T& Get(const vd::uid id) const
	{
		return GetValue(id, Identity<T>());
	}

	template< typename T >
	void Set(const vd::uid id, const T& value)
	{
		SetValue(id, value, Identity<T>());
	}
	
	size_t Size() const { return 1; } // return v3f_.size(); }
	void Clear() { } // v3f_.clear(); }
	
protected:

	template<typename T>
	const T& GetValue(const vd::uid id, Identity<T>) const;	

	template<typename T>
	void SetValue(const vd::uid id, const T& value, Identity<T>);

};

// ============================================================================================== //

#define VD_DECLARE_PROPERTY_TYPE_SUPPORT(name) 												\
public:																						\
	typedef AlignedMap<vd::uid, name, UidHash>::type Map ##name ##Type;						\
																							\
protected:																					\
	Map ##name ##Type map_ ##name ##_;														\
																							\
protected:																					\
																							\
	const name &GetValue(const vd::uid id, Identity< name >) const							\
	{																						\
		return map_ ##name ##_.at(id);														\
	}																						\
																							\
	void SetValue(const vd::uid id, const name & value, Identity< name >)			    	\
	{																						\
		map_ ##name ##_[id] = value;														\
	}																						

*/
// ============================================================================================== //

VD_CONTAINERS_NAMESPACE_END();

// ============================================================================================== //

#endif	// VD_CONTAINERS_PROPERTY_SET_INCLUDED
