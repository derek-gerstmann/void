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

#ifndef VD_CORE_HASHING_INCLUDED
#define VD_CORE_HASHING_INCLUDED

// ============================================================================================== //

#include "core/core.h"

// ============================================================================================== //

VD_CORE_NAMESPACE_BEGIN();

// ============================================================================================== //

struct Hashing 
{

// ============================================================================================== //

template <unsigned int N, unsigned int I>
struct Fnv
{
	VD_FORCE_INLINE static 
	vd::u32 Hash(const char (&str)[N])
	{
		return (Fnv<N, I-1>::Hash(str) ^ str[I-1])*16777619u;
	}
};
 
// ============================================================================================== //

template <unsigned int N>
struct Fnv<N, 1>
{
	VD_FORCE_INLINE static 
	vd::u32 Hash(const char (&str)[N])
	{
		return (2166136261u ^ str[0])*16777619u;
	}
};
 
// ============================================================================================== //

// MurmurHash3 was written by Austin Appleby, and was placed in the public
// domain. The author disclaimed all copyright to this source code.

// Note - The x86 and x64 versions do _not_ produce the same results, as the
// algorithms are optimized for their respective platforms. You can still
// compile and run any of them on any platform, but your performance with the
// non-native version will be less than optimal.

// ============================================================================================== //

	static void MurmurHash3_x86_32  ( const char * key, int len, vd::u32 seed, vd::uid * out );
	static void MurmurHash3_x86_128 ( const char * key, int len, vd::u32 seed, vd::uid * out );
	static void MurmurHash3_x64_128 ( const char * key, int len, vd::u32 seed, vd::uid * out );

// ============================================================================================== //

#if defined( VD_64BIT )

	static inline
	vd::uid Murmur( const char * key, int len, vd::u32 seed = 0xe17a1465)
	{ 
		vd::uid out;
		MurmurHash3_x64_128(key, len, seed, &out); 
		return out;
	}

#else

	static inline
	vd::uid Murmur( const char * key, int len, vd::u32 seed = 0xe17a1465)
	{ 
		vd::uid out;
		MurmurHash3_x86_128(key, len, seed, &out); 
		return out;
	}

#endif

	static vd::u32 
	Whiz(const char *str, size_t wrdlen);
};

// ============================================================================================== //

struct Tiny 
{

// ============================================================================================== //
// Tiny Encryption Algorithm (TEA) was written by David Wheeler and Roger Needham, and was placed in 
// the public domain. The author disclaimed all copyright to this source code.
// ============================================================================================== //

	static
	vd::u64 Encode(vd::u64 v, vd::i32 length);

	static
	vd::u64 Decode(vd::u64 v, vd::i32 length);
};

// ============================================================================================== //

VD_CORE_NAMESPACE_END();

// ============================================================================================== //

#endif	// VD_CORE_HASHING_INCLUDED

