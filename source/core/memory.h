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

class ByteOrder
{
public:
    static bool IsBigEndian ();
    static bool IsLittleEndian ();

    static void Swap2 (void* data);
    static void Swap2 (int count, void* data);
    static void Swap4 (void* data);
    static void Swap4 (int count, void* data);
    static void Swap8 (void* data);
    static void Swap8 (int count, void* data);
    static void Swap (size_t bytes, void* data);
    static void Swap (size_t bytes, int count, void* data);

    template<typename T> static inline 
    T Swap(T value) 
    {
        union {
            T       value;
            vd::u8  swap[sizeof(T)];
        } u;

        u.value = value;
        std::reverse(&u.swap[0], &u.swap[sizeof(T)]);
        return u.value;
    }

private:
    static bool m_IsLittle;
};

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

    static void* SetBytes(void* b, int c, size_t len);
    static void* CopyBytes(void* s1, const void* s2, size_t n);
    static void* MoveBytes(void* s1, const void* s2, size_t n);

#if defined(VD_DEBUG_MEMORY)
    static void* Acquire(void*, size_t bytes, const char*, const char*, int);
    static void  Release(void* ptr);
    static void  Dump(void);
    static void  Startup(void);
    static void  Shutdown(void);
#else
    static inline void* Acquire(void* ptr, size_t bytes, const char*, const char*, int) {return ptr;}
    static inline void  Release(void* ptr) {}
    static inline void  Dump(void) {}
    static inline void  Startup(void) {}
    static inline void  Shutdown(void) {}
#endif // VD_DEBUG_MEMORY

    template <typename T>
    T* NewArray(size_t size) 
    {
        T* ptr = VD_NEW_ARRAY(T, size);
        vdGlobalAssertMsg((ptr != NULL), "Memory: System aligned allocation failed!");
        return ptr;
    }

    template <typename T>
    void DeleteArray(T* array) 
    {
        VD_DELETE_ARRAY(array);
    }

    char* StrDup(const char* str);
    char* StrNDup(const char* str, int n);
     
    template <typename T>
    static inline T* Reserve(T* ptr, size_t bytes, const char* file, const char* function, int line)
    {
        Acquire(ptr, bytes, file, function, line);
        return ptr;
    }
    
    template <typename T1, typename T2, size_t N>
    void CopyBytes(T1* to, const T2* fm)
    {
        struct ByteArray
        {
            char Data[N];
        };

        *(reinterpret_cast<ByteArray*>(to)) = *(reinterpret_cast<const ByteArray*>(fm));
    }

    template <typename T, size_t N>
    void CopyEntries(T* to, const T* fm)
    {
        struct EntryArray
        {
            T Data[N];
        };

        *(reinterpret_cast<EntryArray*>(to)) = *(reinterpret_cast<const EntryArray*>(fm));
    }

    static const size_t SystemAlignment = 4;

    template <class T>
    static inline size_t AlignOf() { struct { char c; T t; } s; return sizeof(s) - sizeof(T); }

    template <class Type, class Field>
    static inline size_t FieldOffsetOf(Field (Type::*field))
    {
        Type *object = 0;
        return uintptr_t(&(object->*field)) - uintptr_t(object);
    }

    static inline size_t AlignUp(size_t size, size_t alignment = SystemAlignment)
    {
        return ((size - 1) & ~(alignment - 1)) + alignment;
    }

    static inline void *AlignUp(void *p, size_t alignment = SystemAlignment)
    {
        return reinterpret_cast<void *>(AlignUp(uintptr_t(p), alignment));
    }

    static inline const void *AlignUp(const void *p, size_t alignment = SystemAlignment)
    {
        return reinterpret_cast<const void *>(AlignUp(uintptr_t(p), alignment));
    }

    template <class T>
    static inline const T *Increment(const void *p, ptrdiff_t offset)
    { return reinterpret_cast<const T *>(uintptr_t(p) + offset); }

    template <class T>
    static inline T *Increment(void *p, ptrdiff_t offset)
    { return reinterpret_cast<T *>(uintptr_t(p) + offset); }

    static inline const void *Increment(const void *p, ptrdiff_t offset)
    { return Increment<const void>(p, offset); }

    static inline void *Increment(void *p, ptrdiff_t offset)
    { return Increment<void>(p, offset); }

    template <class T>
    static inline const T *Increment(const void *p, ptrdiff_t offset, size_t alignment)
    { return Increment<const T>(AlignUp(p, alignment), offset); }

    template <class T>
    static inline T *Increment(void *p, ptrdiff_t offset, size_t alignment)
    { return Increment<T>(AlignUp(p, alignment), offset); }

    static inline const void *Increment(const void *p, ptrdiff_t offset, size_t alignment)
    { return Increment<const void>(p, offset, alignment); }

    static inline void *Increment(void *p, ptrdiff_t offset, size_t alignment)
    { return Increment<void>(p, offset, alignment); }

    static inline ptrdiff_t Difference(const void *p1, const void *p2)
    { return uintptr_t(p1) - uintptr_t(p2); }

};

// ============================================================================================== //

VD_CORE_NAMESPACE_END();

// ============================================================================================== //

#endif 	// VD_CORE_MEMORY_INCLUDED

