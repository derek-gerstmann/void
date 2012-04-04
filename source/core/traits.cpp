// ============================================================================================== //
//
// License:     The Lesser GNU Public License (LGPL) v3.0.
// 
// Author(s):   Derek Gerstmann. The University of Western Australia (UWA). 
//              As well as the shoulders of many giants...
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

#include "core/traits.h"
#include "core/symbol.h"
#include "core/entity.h"
#include "core/threading.h"
#include "core/logging.h"
#include "containers/containers.h"

#include <fstream>
#include <iostream>

// ============================================================================================== //

VD_CORE_NAMESPACE_BEGIN();

// ============================================================================================== //

typedef Entity* (*CreateFn)();
typedef Entity* (*LoadFn)(Stream*, Registry*);

// ============================================================================================== //

bool Traits::m_IsInitialized = false;

// ============================================================================================== //

namespace
{
    static Traits* TraitRegistryHead = NULL;
    static Traits* TraitRegistryTail = NULL;
}

void Traits::RegisterSelf(void)
{
    if(TraitRegistryHead)
    {
        TraitRegistryTail->m_Next = this;
        TraitRegistryTail = this;
    }
    else
    {
        TraitRegistryHead = this;
        TraitRegistryTail = this;
    }
}

Traits::Traits(
    const Symbol& name,
    const Symbol& parent_name,
    bool is_abstract,
    void* createfn,
    void* loadfn
) :
    m_Next(NULL),
    m_Name(name),
    m_ParentName(parent_name),
    m_ParentTraits(NULL),
    m_IsAbstract(is_abstract),
    m_CreateFn(createfn),
    m_LoadFn(loadfn)
{
    RegisterSelf();
}

const Traits* 
Traits::Retrieve(
    const Symbol& symbol)
{
    if(!symbol.IsValid())
        return NULL;

    Traits* t = TraitRegistryHead;
    while(t != NULL)
    {
        if(t->m_Name == symbol)
            return t;
        t = t->m_Next;
    }

    return NULL;
}

bool 
Traits::IsDerivedFrom(
    const Traits* traits) const
{
    const Traits* t = this;

    while(t != NULL)
    {
        if(traits == t)
            return true;

        t = t->GetParentTraits();
    }

    return false;
}

void 
Traits::Register(
    Traits* traits)
{
    Traits* t = TraitRegistryHead;
    if(traits->GetName().IsValid() && traits->m_ParentName.IsValid())
    {
        while(t != NULL)
        {
            if(t->m_Name == traits->m_ParentName)
            {
                traits->m_ParentTraits = t;
                break;  
            }
            t = t->m_Next;
        }

        if( traits->m_ParentTraits == NULL )
        {
            std::cerr << "Critical error during the static RTTI initialization: " << std::endl
                      << "Could not locate the base class '" << traits->m_ParentName.GetStr() << "' while initializing '"
                      << traits->GetName().GetStr() << "'!" << std::endl;
            exit(-1);
        }
    }
}

void Traits::CreateRegistry()
{
    Traits* t = TraitRegistryHead;
    while(t != NULL)
    {
        Register(t);
        t = t->m_Next;
    }
    m_IsInitialized = true;
}

Entity* Traits::Create() const
{
    if(m_CreateFn == NULL)
    {
        vdLogGlobalError("RTTI error: An attempt to instantiate a "
                         "class lacking the instantiation feature occurred (%s)!",
                         GetName().GetStr());
    }

    return ((CreateFn) m_CreateFn)();
}

Entity* Traits::Load(Stream* stream, Registry* registry) const
{
    if(m_LoadFn == NULL)
    {
        vdLogGlobalError("RTTI error: An attempt to instantiate a "
                         "class lacking the unserialization feature occurred (%s)!",
                         GetName().GetStr());
    }

    return ((LoadFn) m_LoadFn)(stream, registry);
}

void Traits::DestroyRegistry()
{
    m_IsInitialized = false;
}

// ============================================================================================== //

VD_CORE_NAMESPACE_END();

// ============================================================================================== //
