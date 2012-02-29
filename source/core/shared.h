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

#ifndef VD_CORE_SHARED_INCLUDED
#define VD_CORE_SHARED_INCLUDED

// ============================================================================================== //

#include "core/core.h"
#include "core/atomics.h"
#include "core/memory.h"

// ============================================================================================== //

VD_CORE_NAMESPACE_BEGIN();

// ============================================================================================== //

/// Base class for non-atomic reference counting support.
template< typename T >
class RawRefCounted
{
public:
    RawRefCounted();
    RawRefCounted(const RawRefCounted& other);
    virtual ~RawRefCounted();

    vd::i32 GetRefCount();
    vd::i32 Retain();
    vd::i32 Release();

    RawRefCounted& operator=(const RawRefCounted& other);

private:
    /// Reference count.
    mutable vd::i32 m_Count;

};

/// Base class for atomic reference counting support.
template< typename T >
class RefCounted
{
public:
    RefCounted();
    RefCounted(const RefCounted& other);
    virtual ~RefCounted();

    vd::i32 GetRefCount();
    vd::i32 Retain();
    vd::i32 Release();

    RefCounted& operator=(const RefCounted& other);

private:
    AtomicCounter m_Count;

};

// ============================================================================================== //

template<typename T>
class Shared : public RefCounted<T>
{
	// EMPTY!
};

// ============================================================================================== //

template< typename T >
RawRefCounted< T >::RawRefCounted()
    : m_Count(0)
{
	// EMPTY!
}

template< typename T >
RawRefCounted< T >::~RawRefCounted()
{
	// EMPTY!
}

template< typename T >
RawRefCounted< T >::RawRefCounted(const RawRefCounted&)
    : m_Count(0)
{
    // DO *NOT* COPY THE REFCOUNT
}

template< typename T >
vd::i32 RawRefCounted< T >::GetRefCount()
{
    return m_Count;
}

template< typename T >
vd::i32 RawRefCounted< T >::Retain()
{
    ++m_Count;
	const vd::i32 usage = m_Count;
//    vdGlobalAssert(usage != 0);
    return usage;
}

template< typename T >
vd::i32 RawRefCounted< T >::Release()
{
    --m_Count;
	const vd::i32 usage = m_Count;
    if(usage == 0)
    {
        VD_DELETE( const_cast< T* >(static_cast< const T* >(this)) );
    }
    return usage;
}

template< typename T >
RawRefCounted< T >& 
RawRefCounted< T >::operator=(const RawRefCounted&)
{
    // DO *NOT* COPY THE REFCOUNT
    return *this;
}

template< typename T >
RefCounted< T >::RefCounted()
    : m_Count(0)
{
	// EMPTY!
}

template< typename T >
RefCounted< T >::RefCounted(const RefCounted& /*other*/)
    : m_Count(0)
{
    // DO *NOT* COPY THE REFCOUNT
}

template< typename T >
RefCounted< T >::~RefCounted()
{
}

template< typename T >
vd::i32 RefCounted< T >::GetRefCount()
{
	const vd::i32 usage = m_Count;
    return usage;
}

template< typename T >
vd::i32 RefCounted< T >::Retain()
{
	const vd::i32 usage = m_Count.Increment();
    return usage;
}

template< typename T >
vd::i32 RefCounted< T >::Release()
{
	const vd::i32 usage = m_Count.Decrement();
    if(usage == 0)
    {
        VD_DELETE( this );
    }

    return usage;
}

template< typename T >
RefCounted< T >& 
RefCounted< T >::operator=(const RefCounted& /*other*/)
{
    // DO *NOT* COPY THE REFCOUNT
    return *this;
}

// ============================================================================================== //

VD_CORE_NAMESPACE_END();

// ============================================================================================== //

#endif // VD_CORE_SHARED_INCLUDED
