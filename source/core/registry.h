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

#ifndef VD_CORE_REGISTRY_INCLUDED
#define VD_CORE_REGISTRY_INCLUDED

// ============================================================================================== //

#include "core/core.h"

// ============================================================================================== //

VD_CORE_NAMESPACE_BEGIN();

// ============================================================================================== //

template<class T>
class ValueRegistry
{
public:
	typedef T ValueType;
	static const vd::u32 Capacity = 64*1024;
	static const vd::u32 IndexMask = 0xFFFF;
	static const vd::u32 IndexOffset = 0x10000;
	static const vd::u32 InvalidIndex = VD_U16_MAX;

	struct Index 
	{
		vd::u32 Id;
		vd::u16 Index;
		vd::u16 Next;
	};

	ValueRegistry() 
	:	m_Size(0),
		m_FreeListHead(0),
		m_FreeListTail(0)
	{
		// EMPTY!
	}		
		
	void Setup(void) 
	{
		Clear();
	}
		
	void Clear(void) 
	{
		m_Size = 0;
		for (vd::u32 i = 0; i < Capacity; ++i) 
		{
			m_IndexTable[i].Id = i;
			m_IndexTable[i].Next = i+1;
		}
		m_FreeListTail = 0;
		m_FreeListHead = Capacity-1;
	}

	inline bool 
	Exists(vd::u32 id) 
	{
		Index &in = m_IndexTable[id & IndexMask];
		return (in.Id == id) && (in.Index != InvalidIndex);
	}
	
	inline ValueType& 
	Retrieve(vd::u32 id) 
	{
		return m_EntryTable[m_IndexTable[(id & IndexMask)].Index];
	}
	
	inline vd::u32 
	Reserve() 
	{
		Index &in = m_IndexTable[m_FreeListTail];
		m_FreeListTail = in.Next;
		in.Id += IndexOffset;
		in.Index = m_Size++;
		ValueType &v = m_EntryTable[in.Index];
		v.SetId(in.Id);
		return v.GetId();
	}
	
	inline void 
	Remove(vd::u32 id) 
	{
		Index &in = m_IndexTable[id & IndexMask];
		
		ValueType &v = m_EntryTable[in.Index];
		v = m_EntryTable[--m_Size];
		m_IndexTable[v.GetId() & IndexMask].Index = in.Index;
		
		in.Index = InvalidIndex;
		m_IndexTable[m_FreeListHead].Next = id & IndexMask;
		m_FreeListHead = id & IndexMask;
	}
	
	vd::u32 Size()
	{
		return m_Size;
	}

protected:
	vd::u32 	m_Size;
	vd::u16 	m_FreeListHead;
	vd::u16 	m_FreeListTail;
	ValueType 	m_EntryTable[Capacity];
	Index 		m_IndexTable[Capacity];
};

// ============================================================================================== //

VD_CORE_NAMESPACE_END();

// ============================================================================================== //

#endif // VD_CORE_REGISTRY_INCLUDED

