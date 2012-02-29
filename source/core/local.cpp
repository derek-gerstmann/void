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

#include "core/local.h"
#include "core/handles.h"
#include "core/logging.h"
#include "core/asserts.h"

// ============================================================================================== //

VD_CORE_NAMESPACE_BEGIN();

// ============================================================================================== //

LocalStorageKey::LocalStorageKey(
	DestroyCallback cb) 
: 	m_Handle(NULL),
 	m_Destructor(cb)
{
	Create(cb);
}

// ============================================================================================== //

LocalStorageKey::~LocalStorageKey()
{
	Destroy();
}

// ============================================================================================== //

void LocalStorageKey::Set(void* ptr)
{
	vdGlobalAssertMsg(m_Handle != NULL, "Attempted to Set thread local storage value before its been created!");
	
#if defined(VD_TARGET_WINDOWS)
    
    ::DWORD key = reinterpret_cast<DWORD>(m_Handle);
    if(!::TlsSetValue(key, ptr)) 
    	vdGlobalException("TlsSetValue", "Failed to set thread local storage value!");

#elif defined(VD_USE_POSIX)
    
    ::pthread_key_t* key = reinterpret_cast<pthread_key_t*>(m_Handle);
    if(::pthread_setspecific(*key, ptr) != 0)
    	vdGlobalException("pthread_setspecific", "Failed to set thread local storage value!");

#endif
}

// ============================================================================================== //

void* LocalStorageKey::Get() const
{
	vdGlobalAssertMsg(m_Handle != NULL, "Attempted to Get value from thread local storage before its been created!");
		
#if defined(VD_TARGET_WINDOWS)
    
    ::DWORD key = reinterpret_cast<DWORD>(m_Handle);
    return ::TlsGetValue(key);

#elif defined(VD_USE_POSIX)
    
    ::pthread_key_t* key = reinterpret_cast<pthread_key_t*>(m_Handle);
    return ::pthread_getspecific(*key);

#endif
}

void LocalStorageKey::Create(
	DestroyCallback cb)
{
	Destroy();
	
	m_Destructor = cb;
	
#if defined(VD_TARGET_WINDOWS)
    
    ::DWORD key = ::TlsAlloc();
    if(key == 0xFFFFFFFF) 
    	vdGlobalException("TlsAlloc", "Failed to create thread local storage!");

    m_Handle = reinterpret_cast<void*>(key);

#elif defined(VD_USE_POSIX)
    
    ::pthread_key_t* key = VD_NEW( pthread_key_t );
    if(::pthread_key_create(key, m_Destructor) != 0)
    	vdGlobalException("pthread_key_create", "Failed to create thread local storage!");

    m_Handle = reinterpret_cast<void*>(key);

#endif

}

void LocalStorageKey::Destroy()
{
	if(m_Handle == NULL)
		return;
		
#if defined(VD_TARGET_WINDOWS)
    
    ::DWORD key = (::DWORD)m_Handle;
    ::TlsFree(key);

#elif defined(VD_USE_POSIX)
    
    ::pthread_key_t* key = (::pthread_key_t*)m_Handle;
    ::pthread_key_delete(*key);
    VD_DELETE(key);

#endif

	m_Handle = NULL;
	m_Destructor = NULL;
}

// ============================================================================================== //

VD_CORE_NAMESPACE_END();

// ============================================================================================== //

