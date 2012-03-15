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
#include "core/logging.h"
#include "containers/containers.h"

#include <fstream>
#include <iostream>

// ============================================================================================== //

VD_CORE_NAMESPACE_BEGIN();

// ============================================================================================== //

typedef Object* (*CreateFn)();
typedef Object* (*LoadFn)(Stream*, InstanceRegistry*);

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
            std::cerr << "Critical error during the static RTTI initialization: " << std::endl
                      << "Could not locate the base class '" << metaclass->m_ParentName.ToString() << "' while initializing '"
                      << metaclass->GetIdentifier().ToString() << "'!" << std::endl;
            exit(-1);
        }
    }
}

void MetaClass::CreateRegistry()
{
    MetaClass* mc = ClassRegistryHead;
    while(mc != NULL)
    {
     	Register(mc);
	    mc = mc->m_Next;
    }
    m_IsInitialized = true;
}

Object* MetaClass::Create() const
{
    if(m_CreateFn == NULL)
    {
        vdLogGlobalError("RTTI error: An attempt to instantiate a "
                         "class lacking the instantiation feature occurred (%s)!",
                         GetIdentifier().ToString());
    }

    return ((CreateFn) m_CreateFn)();
}

Object* MetaClass::Load(Stream* stream, InstanceRegistry* instances) const
{
    if(m_LoadFn == NULL)
    {
        vdLogGlobalError("RTTI error: An attempt to instantiate a "
                         "class lacking the unserialization feature occurred (%s)!",
                         GetIdentifier().ToString());
    }

    return ((LoadFn) m_LoadFn)(stream, instances);
}

void MetaClass::DestroyRegistry()
{
    m_IsInitialized = false;
}

// ============================================================================================== //

VD_CORE_NAMESPACE_END();

// ============================================================================================== //
