// ============================================================================================== //
//
// License:   The Lesser GNU Public License (LGPL) v3.0.
//
// Author(s):   Derek Gerstmann. The University of Western Australia (UWA).
//        As well as the shoulders of many giants...
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

#ifndef VD_CORE_REFERENCES_INCLUDED
#define VD_CORE_REFERENCES_INCLUDED

// ============================================================================================== //

#include "core/core.h"
#include "core/atomics.h"
#include "core/memory.h"
#include "constants/constants.h"

// ============================================================================================== //

VD_CORE_NAMESPACE_BEGIN();

// ============================================================================================== //

class RefCount
{
public:
    RefCount() : m_Counter(0) {}
    virtual ~RefCount() {}
    VD_FORCE_INLINE void Retain() { m_Counter++; }
    VD_FORCE_INLINE bool Release() { return !(--m_Counter); }
    AtomicCounter m_Counter;
};

// ============================================================================================== //

using VD_CONSTANTS_NAMESPACE::NullType;

// ============================================================================================== //

template<typename Type>
class Reference
{

public:
    const Type* Ptr;

    VD_FORCE_INLINE Reference(void) : Ptr(NULL) {}
    VD_FORCE_INLINE Reference(NullType) : Ptr(NULL) {}
    VD_FORCE_INLINE Reference(const Reference& input) : Ptr(input.Ptr) { if(Ptr) Ptr->Retain(); }
    VD_FORCE_INLINE Reference(Type* const input) : Ptr(input) { if(Ptr) Ptr->Retain(); }
    VD_FORCE_INLINE ~Reference(void) { if(Ptr && Ptr->Release()) VD_DELETE(Ptr); }

    VD_FORCE_INLINE Reference& operator= (const Reference& input)
    {
        if(input.Ptr) input.Ptr->Retain();

        if(Ptr && Ptr->Release()) VD_DELETE(Ptr);

        *(Type**)&Ptr = input.Ptr;
        return *this;
    }

    VD_FORCE_INLINE Reference& operator= (NullType)
    {
        if(Ptr && Ptr->Release()) VD_DELETE(Ptr);

        *(Type**)&Ptr = NULL;
        return *this;
    }

    VD_FORCE_INLINE operator bool(void) const { return Ptr != NULL; }
    VD_FORCE_INLINE const Type& operator* (void) const { return *Ptr; }
    VD_FORCE_INLINE const Type* operator-> (void) const { return  Ptr; }
    VD_FORCE_INLINE Type& operator* (void) { return *Ptr; }
    VD_FORCE_INLINE Type* operator-> (void) { return  Ptr; }

    template<typename CastType> VD_FORCE_INLINE
    Reference<CastType> StaticCast()
    { return Reference<CastType>(static_cast<CastType*>(Ptr)); }

    template<typename CastType> VD_FORCE_INLINE
    const Reference<CastType> StaticCast() const
    { return Reference<CastType>(static_cast<CastType*>(Ptr)); }
};

// ============================================================================================== //

template<typename Type> VD_FORCE_INLINE
bool operator< (const Reference<Type>& a, const Reference<Type>& b) { return a.Ptr <  b.Ptr ; }

template<typename Type> VD_FORCE_INLINE
bool operator== (const Reference<Type>& a, NullType) { return a.Ptr == NULL  ; }

template<typename Type> VD_FORCE_INLINE
bool operator== (NullType, const Reference<Type>& b) { return NULL  == b.Ptr ; }

template<typename Type> VD_FORCE_INLINE
bool operator== (const Reference<Type>& a, const Reference<Type>& b) { return a.Ptr == b.Ptr ; }

template<typename Type> VD_FORCE_INLINE
bool operator!= (const Reference<Type>& a, NullType) { return a.Ptr != NULL  ; }

template<typename Type> VD_FORCE_INLINE
bool operator!= (NullType, const Reference<Type>& b) { return NULL  != b.Ptr ; }

template<typename Type> VD_FORCE_INLINE
bool operator!= (const Reference<Type>& a, const Reference<Type>& b) { return a.Ptr != b.Ptr ; }

// ============================================================================================== //

VD_CORE_NAMESPACE_END();

// ============================================================================================== //

#endif // VD_CORE_REFERENCES_INCLUDED

