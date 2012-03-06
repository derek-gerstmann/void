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

#ifndef VD_CORE_HANDLES_INCLUDED
#define VD_CORE_HANDLES_INCLUDED

// ============================================================================================== //

#include "core/core.h"

// ============================================================================================== //

VD_CORE_NAMESPACE_BEGIN();

// ============================================================================================== //

template <typename T> 
class Handle
{
public:
	typedef T ValueType;
	
    Handle() : m_Ptr(NULL) { }
    Handle(T* ptr) : m_Ptr(ptr) { if(m_Ptr) m_Ptr->Retain(); }
    Handle(const Handle& pRef) : m_Ptr(pRef.m_Ptr) { if(m_Ptr) m_Ptr->Retain(); }
    ~Handle() { if(m_Ptr) m_Ptr->Release(); }

    inline Handle& operator= (const Handle& pHandle)
    {
        if(m_Ptr == pHandle.m_Ptr)
            return *this;

        T* tmp = m_Ptr;
        m_Ptr = pHandle.m_Ptr;

        if(m_Ptr)
            m_Ptr->Retain();

        if(tmp)
            tmp->Release();

        return *this;
    }

    inline Handle& operator= (T* ptr)
    {
        if(m_Ptr == ptr)
            return *this;

        T* tmp = m_Ptr;
        m_Ptr = ptr;

        if(m_Ptr)
            m_Ptr->Retain();

        if(tmp)
            tmp->Release();

        return *this;
    }

    inline bool operator== (const Handle& pHandle) const { return (m_Ptr == pHandle.m_Ptr); }
    inline bool operator!= (const Handle& pHandle) const { return (m_Ptr != pHandle.m_Ptr); }
    inline bool operator== (const T* ptr) const { return (m_Ptr == ptr); }
    inline bool operator!= (const T* ptr) const { return (m_Ptr != ptr); }
    inline bool operator!() const { return (m_Ptr == NULL); }
    inline T* operator-> () { return m_Ptr; }
    inline const T* operator-> () const { return m_Ptr; }
    inline T& operator*() { return *m_Ptr; }
    inline const T& operator*() const { return *m_Ptr; }
    inline operator T* () { return m_Ptr; }
    inline T* Get() { return m_Ptr; }
    inline const T* Get() const { return m_Ptr; }

    inline vd::string ToString() const
    {
#if 0
        if(m_Ptr == NULL)
            return FormatString("Handle<%s>[null]",
                                T::Class->GetName().c_str());
        else
            return FormatString("Handle<%s>[Handle=%i, ptr=%s]",
                                m_Ptr->GetMetaClass()->GetName().c_str(),
                                m_Ptr->GetRefCount(), m_Ptr->ToString().c_str());
#endif
        return vd::string("Handle");
    }
private:
    T* m_Ptr;
};

// ============================================================================================== //

template <typename T>
class HandleComparator
{
public:
    HandleComparator()
        : m_Ptr(NULL)
    {
    }

    HandleComparator(const Handle<T>& other)
        : m_Ptr(other)
    {
    }

    HandleComparator(const T* other)
        : m_Ptr(other)
    {
    }

    inline bool operator<(const HandleComparator& other) const
    {
        return (*m_Ptr) < (*other.m_Ptr);
    }

    inline bool operator==(const HandleComparator& other) const
    {
        return (*m_Ptr) == (*other.m_Ptr);
    }

    const T* operator->() const
    {
        return m_Ptr.operator->();
    }

    T* operator->()
    {
        return m_Ptr.operator->();
    }

    operator const T* () const
    {
        return m_Ptr.operator T * ();
    }

    operator T* ()
    {
        return m_Ptr.operator T * ();
    }

    operator const Handle<T> () const
    {
        return m_Ptr;
    }

    operator Handle<T> ()
    {
        return m_Ptr;
    }

private:
    Handle<T> m_Ptr;
};

// ============================================================================================== //

VD_CORE_NAMESPACE_END();

// ============================================================================================== //

#endif // VD_CORE_HANDLES_INCLUDED

// ============================================================================================== //
// END OF FILE




