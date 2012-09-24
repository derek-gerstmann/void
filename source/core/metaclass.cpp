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

#include "core/metaclass.h"
#include "core/symbol.h"
#include "core/object.h"
#include "core/threading.h"
#include "core/asserts.h"
#include "core/logging.h"
#include "core/stream.h"
#include "core/repository.h"
#include "core/collections.h"

#include <fstream>
#include <iostream>

// ============================================================================================== //

VD_CORE_NAMESPACE_BEGIN();

// ============================================================================================== //

typedef Object* (*CreateFn)();
typedef Object* (*LoadFn)(Stream*, Repository*);

// ============================================================================================== //

bool MetaClass::m_IsInitialized = false;

// ============================================================================================== //

namespace
{
	static MetaClass* ClassRegistryHead = NULL;
	static MetaClass* ClassRegistryTail = NULL;
}

void MetaClass::RegisterSelf(void)
{
    if(ClassRegistryHead)
    {
        ClassRegistryTail->m_Next = this;
        ClassRegistryTail = this;
    }
    else
    {
        ClassRegistryHead = this;
        ClassRegistryTail = this;
    }
}

MetaClass::MetaClass(
    const vd::symbol& name,
    const vd::symbol& parent_name,
    bool is_abstract,
    void* createfn,
    void* loadfn
) :
	m_Next(NULL),
    m_Name(name),
    m_ParentName(parent_name),
    m_ParentClass(NULL),
    m_IsAbstract(is_abstract),
    m_CreateFn(createfn),
    m_LoadFn(loadfn)
{
    RegisterSelf();
}

const MetaClass* 
MetaClass::Retrieve(
	const vd::symbol& symbol)
{
    if(!symbol.IsValid())
    	return NULL;

    MetaClass* mc = ClassRegistryHead;
    while(mc != NULL)
    {
    	if(mc->m_Name == symbol)
    		return mc;
	    mc = mc->m_Next;
    }

    return NULL;
}

const MetaClass* 
MetaClass::Retrieve(
    const Core::Symbol& symbol)
{
    if(!symbol.IsValid())
        return NULL;

    return Retrieve(vd::symbol(symbol.GetKey(), symbol.GetStr()));
}

bool 
MetaClass::IsDerivedFrom(
	const MetaClass* metaclass) const
{
    const MetaClass* mc = this;

    while(mc != NULL)
    {
        if(metaclass == mc)
            return true;

        mc = mc->GetParentClass();
    }

    return false;
}

void 
MetaClass::Register(
	MetaClass* metaclass)
{
    MetaClass* mc = ClassRegistryHead;
    if(metaclass->GetIdentifier().IsValid() && metaclass->m_ParentName.IsValid())
    {
		while(mc != NULL)
		{
			if(mc->m_Name == metaclass->m_ParentName)
			{
				metaclass->m_ParentClass = mc;
				break;	
			}
			mc = mc->m_Next;
		}

		if(	metaclass->m_ParentClass == NULL )
		{
            std::cerr << "ERROR: Static runtime initialization failed! " << std::endl
                      << "Could not locate the base class '" << metaclass->m_ParentName.ToString() << "' while initializing '"
                      << metaclass->GetIdentifier().ToString() << "'!" << std::endl;
            exit(-1);
        }
    }
}

void MetaClass::CreateRegistry()
{
    Core::Mutex mutex;
    mutex.Lock();
    MetaClass* mc = ClassRegistryHead;
    while(mc != NULL)
    {
     	Register(mc);
	    mc = mc->m_Next;
    }
    m_IsInitialized = true;
    mutex.Unlock();
}

Object* MetaClass::Create() const
{
    vdGlobalAssertMsg((m_CreateFn != NULL),
        "ERROR: Attempted to instantiate an object without factory method (%s)!",
        GetIdentifier().ToString());

    return ((CreateFn) m_CreateFn)();
}

Object* MetaClass::Load(Stream* stream, Repository* repo) const
{
    vdGlobalAssertMsg((m_LoadFn != NULL),
        "ERROR: Attempted to instantiate an object without a load method (%s)!",
        GetIdentifier().ToString());


    return ((LoadFn) m_LoadFn)(stream, repo);
}

void MetaClass::DestroyRegistry()
{
    m_IsInitialized = false;
}

// ============================================================================================== //

VD_CORE_NAMESPACE_END();

// ============================================================================================== //
