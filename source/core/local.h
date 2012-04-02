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

#ifndef VD_CORE_LOCAL_INCLUDED
#define VD_CORE_LOCAL_INCLUDED

// ============================================================================================== //

#include "core/core.h"
#include "core/memory.h"

// ============================================================================================== //

VD_CORE_NAMESPACE_BEGIN();

// ============================================================================================== //

class LocalStorageKey
{
public:
	typedef void (*DestroyCallback)(void*);
	
    explicit LocalStorageKey(DestroyCallback cb = NULL);
    ~LocalStorageKey();

    void Set(void* ptr);
    void* Get() const;
    
    void Create(DestroyCallback cb = NULL);
    void Destroy();

private:
    void* m_Handle;
    DestroyCallback m_Destructor;
};

// ============================================================================================== //

template <class T>
class LocalStorage
{
public:
	typedef T ValueType;
	
    explicit LocalStorage() : 
    	m_Key(Release)
    {

    }

    ~LocalStorage()
    {
        ValueType* obj = (ValueType*)m_Key.Get();

        if(obj)
        {
            VD_DELETE( obj );
            m_Key.Set(NULL);
        }
    }

    ValueType& operator *()
    {
        ValueType* obj = (ValueType*)m_Key.Get();

        if(!obj)
        {
            obj = VD_NEW( ValueType );
            m_Key.Set(obj);
        }

        return *obj;
    }

    ValueType* operator ->()
    {
        ValueType* obj = (ValueType*)m_Key.Get();

        if(!obj)
        {
            obj = VD_NEW( ValueType );
            m_Key.Set(obj);
        }

        return obj;
    }

    operator ValueType() const
    {
        ValueType* obj = (ValueType*)m_Key.Get();

        if(!obj) return ValueType();
        return *obj;
    }

private:

    static void Release(void* obj)
    {
        VD_DELETE((ValueType*)obj);
    }

    VD_DISABLE_COPY_CONSTRUCTORS(LocalStorage);

private:

    LocalStorageKey m_Key;
};

// ============================================================================================== //

template <typename T> 
class LocalHandle
{
public:
	typedef T ValueType;

    explicit LocalHandle() :
    	m_Key(Release)
    {
        // EMPTY!
    }

    inline ~LocalHandle()
    {
        Destroy();
    }

	inline void Create() 
	{
		m_Key.Create(Release);
	}
	
	void Destroy()
    {
        Set(NULL);
    }

    inline void Set(ValueType* value)
    {
        ValueType* previous = Get();

        if(previous != NULL)
            previous->Release();

        if(value != NULL)
            value->Retain();

        m_Key.Set(value);
    }

    inline ValueType* Get()
    {
        return static_cast<ValueType*>(m_Key.Get());
    }

    inline const ValueType* Get() const
    {
        return static_cast<const ValueType*>(m_Key.Get());
    }

    ValueType& operator *()
    {
        ValueType* obj = Get();
        if(!obj)
        {
            obj = VD_NEW( ValueType );
            m_Key.Set(obj);
        }
        return *obj;
    }

    ValueType* operator ->()
    {
        ValueType* obj = Get();
        if(!obj)
        {
            obj = VD_NEW( ValueType );
            m_Key.Set(obj);
        }
        return obj;
    }

    operator ValueType() const
    {
        ValueType* obj = Get();
        if(!obj) return ValueType();
        return *obj;
    }

private:

    static void Release(void* ptr)
    {
        static_cast<ValueType*>(ptr)->Release();
    }

    VD_DISABLE_COPY_CONSTRUCTORS(LocalHandle);

private:
    LocalStorageKey m_Key;
};

// ============================================================================================== //

VD_CORE_NAMESPACE_END();

// ============================================================================================== //

#endif  // VD_CORE_LOCAL_INCLUDED                     

// ============================================================================================== //

// END OF FILE
