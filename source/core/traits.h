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

#ifndef VD_CORE_TRAITS_INCLUDED
#define VD_CORE_TRAITS_INCLUDED

// ============================================================================================== //

#include "core/core.h"
#include "core/symbol.h"
#include "core/registry.h"

// ============================================================================================== //

VD_CORE_NAMESPACE_BEGIN();

// ============================================================================================== //

class Entity;
class Symbol;
class Stream;

// ============================================================================================== //

typedef ValueRegistry<Entity> Registry;

// ============================================================================================== //

class Traits
{
public:
    Traits(
        const Symbol& name, 
        const Symbol& parent,
        bool is_abstract = true, 
        void* createfn = NULL, 
        void* loadfn = NULL);

    inline const Symbol& GetName() const { return m_Name; }
    inline bool IsAbstract() const { return m_IsAbstract; }
    inline bool IsInstantiable() const { return m_CreateFn != NULL; }
    inline bool IsSerializable() const { return m_LoadFn != NULL; }
    inline const Traits* GetParentTraits() const { return m_ParentTraits; }
    bool IsDerivedFrom(const Traits* metaclass) const;

    Entity* Load(Stream* stream = NULL, Registry* registry = NULL) const;
    Entity* Create() const;

    static void CreateRegistry();
    static void DestroyRegistry();
    static const Traits* Retrieve(const Symbol& name);
    static void Register(Traits* metaclass);
    static inline bool HasRegistry() { return m_IsInitialized; }

private:
    void RegisterSelf(void);
    Traits* m_Next;
    
    Symbol m_Name;
    Symbol m_ParentName;
    Traits* m_ParentTraits;
    bool m_IsAbstract;
    void* m_CreateFn;
    void* m_LoadFn;
    static bool m_IsInitialized;
};

// ============================================================================================== //

VD_CORE_NAMESPACE_END();

// ============================================================================================== //

#endif // VD_CORE_TRAITS_INCLUDED

