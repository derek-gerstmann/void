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

#ifndef VD_CORE_ALLOCATORS_INCLUDED
#define VD_CORE_ALLOCATORS_INCLUDED

// ============================================================================================== //

#include "core/core.h"
#include "core/memory.h"

// ============================================================================================== //

VD_CORE_NAMESPACE_BEGIN();

// ============================================================================================== //

template <typename T>
class RawAllocator
{
public:
	// Typedefs
	typedef size_t    size_type;
	typedef ptrdiff_t difference_type;
	typedef T*        pointer;
	typedef const T*  const_pointer;
	typedef T&        reference;
	typedef const T&  const_reference;
	typedef T         value_type;

public: 
	// Constructors
	RawAllocator() throw(){}
	RawAllocator( const RawAllocator& ) throw(){} 
	template <typename U>
	RawAllocator( const RawAllocator<U>& ) throw(){}
	RawAllocator& operator=( const RawAllocator& ){return *this;}

	// Destructor
	~RawAllocator() throw(){}

	// Utility functions
	pointer address( reference r ) const{return &r;}
	const_pointer address( const_reference c ) const{return &c;} 
	size_type max_size() const{ return std::numeric_limits<size_t>::max() / sizeof(T); }

	// In-place construction
	void construct( pointer p, const_reference c )
	{                                   
	    new( reinterpret_cast<void*>(p) ) T(c);
//		VD_NEW(( reinterpret_cast<void*>(p) ) T, c);
	}

	// In-place destruction
	void destroy( pointer p )
	{                           
		(p)->~T();    // call destructor directly
	}

	// Rebind to allocators of other types
	template <typename U>
	struct rebind
	{
		typedef RawAllocator<U> other;
	};

	// Allocate raw memory
	void deallocate(pointer p, size_type)
	{
		if(p) Memory::RawFree((void*)p);
	}

	pointer allocate(size_type n, const void * = NULL)
	{
		void* p = Memory::RawMalloc(sizeof(T)*n);
		if(p==NULL)throw std::bad_alloc();
		return pointer(p);
	}
}; 

// ============================================================================================== //

// Comparison
template <typename T1, typename T2>
bool operator==( const RawAllocator<T1>&, const RawAllocator<T2>& ) throw(){return true;}

template <typename T1, typename T2>
bool operator!=( const RawAllocator<T1>&, const RawAllocator<T2>& ) throw(){return false;}

// ============================================================================================== //

template <typename T, size_t Sz = 64>
class AlignedAllocator
{
public:
	// Typedefs
	typedef size_t    size_type;
	typedef ptrdiff_t difference_type;
	typedef T*        pointer;
	typedef const T*  const_pointer;
	typedef T&        reference;
	typedef const T&  const_reference;
	typedef T         value_type;

public: 
	// Constructors
	AlignedAllocator() throw(){}
	AlignedAllocator( const AlignedAllocator& ) throw(){} 
	template <typename U>
	AlignedAllocator( const AlignedAllocator<U>& ) throw(){}
	AlignedAllocator& operator=( const AlignedAllocator& ){return *this;}

	// Destructor
	~AlignedAllocator() throw(){}

	// Utility functions
	pointer address( reference r ) const{return &r;}
	const_pointer address( const_reference c ) const{return &c;} 
	size_type max_size() const{ return std::numeric_limits<size_t>::max() / sizeof(T); }

	// In-place construction
	void construct( pointer p, const_reference c )
	{                                   
	    new( reinterpret_cast<void*>(p) ) T(c);
	}

	// In-place destruction
	void destroy( pointer p )
	{                           
		(p)->~T();    // call destructor directly
	}

	// Rebind to allocators of other types
	template <typename U>
	struct rebind
	{
		typedef AlignedAllocator<U> other;
	};

	// Allocate raw memory
	void deallocate(pointer p, size_type)
	{
		if(p) Memory::AlignedFree((void*)p);
	}

	pointer allocate(size_type n, const void * = NULL)
	{
		void* p = Memory::AlignedMalloc(sizeof(T)*n,Sz);
		if(p==NULL)throw std::bad_alloc();
		return pointer(p);
	}
}; 

// ============================================================================================== //

// Comparison
template <typename T1, typename T2>
bool operator==( const AlignedAllocator<T1>&, const AlignedAllocator<T2>& ) throw(){return true;}

template <typename T1, typename T2>
bool operator!=( const AlignedAllocator<T1>&, const AlignedAllocator<T2>& ) throw(){return false;}

// ============================================================================================== //

VD_CORE_NAMESPACE_END();

// ============================================================================================== //

#endif // VD_CORE_ALLOCATORS_INCLUDED

// ============================================================================================== //
// END OF FILE

