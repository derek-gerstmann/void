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

#ifndef VD_CORE_MEMORY_INCLUDED
#define VD_CORE_MEMORY_INCLUDED

// ============================================================================================== //

#include "core/core.h"
#include <cstdlib>

// ============================================================================================== //

VD_CORE_NAMESPACE_BEGIN();

// ============================================================================================== //

class Memory
{
public:

    static void* RawMalloc(size_t size);
    static void* RawRealloc(void* ptr, size_t size);
    static void  RawFree(void* ptr);

    static void* AlignedMalloc(size_t size, size_t align = 64);
    static void* AlignedRealloc(void* ptr, size_t size, size_t align = 64);
    static void  AlignedFree(void* ptr);

	static void* MemSet(void* b, int c, size_t len);
	static void* MemCopy(void* s1, const void* s2, size_t n);
	static void* MemMove(void* s1, const void* s2, size_t n);
     
#if defined(VD_DEBUG_MEMORY)
    static void* Track(void*, size_t bytes, const char*, const char*, int);
    static void  Ignore(void* ptr);
    static void  Dump(void);
    static void  Startup(void);
    static void  Shutdown(void);
#else
    static inline void* Track(void* ptr, size_t bytes, const char*, const char*, int) {return ptr;}
    static inline void  Ignore(void* ptr) {}
    static inline void  Dump(void) {}
    static inline void  Startup(void) {}
    static inline void  Shutdown(void) {}
#endif // VD_DEBUG_MEMORY

    template <typename T>
    static inline T* TrackAlloc(T* ptr, size_t bytes, const char* file, const char* function, int line)
    {
        Track(ptr, bytes, file, function, line);
        return ptr;
    }
};

// ============================================================================================== //

template <int Size, typename T1, typename T2>
void CopyBytes(T1* to, const T2* fm)
{
    struct ByteArray
    {
        char Data[Size];
    };

    *(reinterpret_cast<ByteArray*>(to)) = *(reinterpret_cast<const ByteArray*>(fm));
}

template <int Count, typename T>
void CopyEntries(T* to, const T* fm)
{
    struct EntryArray
    {
        T Data[Count];
    };

    *(reinterpret_cast<EntryArray*>(to)) = *(reinterpret_cast<const EntryArray*>(fm));
}

// ============================================================================================== //

VD_CORE_NAMESPACE_END();

// ============================================================================================== //

#endif 	// VD_CORE_MEMORY_INCLUDED

