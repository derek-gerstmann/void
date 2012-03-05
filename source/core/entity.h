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

#ifndef VD_CORE_ENTITY_INCLUDED
#define VD_CORE_ENTITY_INCLUDED

// ============================================================================================== //

#include "core/core.h"
#include "core/status.h"
#include "core/shared.h"
#include "core/atomics.h"
#include "core/traits.h"

// ============================================================================================== //

VD_CORE_NAMESPACE_BEGIN();

// ============================================================================================== //

class Entity : public Shared<Entity>
{
public:
    Entity(const Entity* parent=NULL);

    virtual vd::status Destroy(void);
    void SetId(vd::uid id);
    vd::uid GetId() const;
    virtual vd::string ToString() const;

    static const char* GetName()
    { 
        static const char* ident = "Entity";
        return ident; 
    };

    static const char* GetResolvedName()
    { 
        static const char* ident = "Entity";
        return ident; 
    };

    virtual Core::Entity* Resolve( const char* identifier )
    {
        return NULL;
    }
    
    virtual const Core::Traits* GetTraits() const;
    static Core::Traits GlobalTraits;

protected:
    virtual ~Entity();
    
protected:
    vd::uid m_Id;
};

// ============================================================================================== //

template< class T >
T* vd_cast( Core::Entity* entity )
{
    if ( !entity ) return NULL;
    Core::Entity* other = entity->Resolve( T::GetResolvedName() );
    if ( !other ) return NULL;
    return static_cast<T*>(other);
}

template< class T >
bool vd_isa( Core::Entity* entity )
{
    if ( entity == NULL ) return false;
    Core::Entity* other = entity->Resolve( T::GetResolvedName() );
    if ( other != NULL ) return true;
    return false;
}

// ============================================================================================== //
/// Entity and Class Declaration Macros
// ============================================================================================== //

#define VD_TRAITS(x) x::GlobalTraits

// ============================================================================================== //

#define VD_RESOLVE_ENTITY( Derived, Base )                                  \
                                                                            \
    static const char* GetBaseName()                                        \
    {                                                                       \
        static const char* ident = #Base;                                   \
        return ident;                                                       \
    };                                                                      \
                                                                            \
    static const char* GetDerivedName()                                     \
    {                                                                       \
        static const char* ident = #Derived;                                \
        return ident;                                                       \
    };                                                                      \
                                                                            \
    static const char* GetResolvedName()                                    \
    {                                                                       \
        static const char* ident = #Base ":" #Derived;                      \
        return ident;                                                       \
    };                                                                      \
                                                                            \
    virtual Entity* Resolve( const char* identifier )                       \
    {                                                                       \
        if ( GetResolvedName() == identifier )                              \
            return this;                                                    \
                                                                            \
        return Base::Resolve( identifier );                                 \
    }

// ============================================================================================== //

#define VD_DERIVE_ENTITY( Derived, Base )                                   \
    public:                                                                 \
        typedef Base BaseType;                                              \
        typedef Derived DerivedType;                                        \
        VD_RESOLVE_ENTITY( Derived, Base )                                  \
        Derived( Entity* parent=NULL )

#define VD_DECLARE_DERIVED_ENTITY( Derived, Base )                          \
    VD_DERIVE_ENTITY( Derived, Base ) : BaseClass( parent )

#define VD_ENTITY_CONSTRUCTOR( Derived )                                    \
    Derived::Derived( Core::Entity* parent ) : BaseClass( parent )

// ============================================================================================== //

#define VD_DECLARE_ENTITY( Derived ) \
public: \
    virtual const Core::Traits* GetTraits() const; \
    static Core::Traits GlobalTraits;

#define VD_IMPLEMENT_ABSTRACT_ENTITY(entity, name, parent) \
    Core::Traits entity::GlobalTraits = Core::Traits(name, parent, true); \
    const Core::Traits* entity::GetTraits() const { \
        return &GlobalTraits;\
    }

#define VD_IMPLEMENT_ENTITY(entity, name, parent) \
    Core::Traits entity::GlobalTraits = Core::Traits(name, parent, false); \
    const Core::Traits* entity::GetTraits() const { \
        return &GlobalTraits;\
    }

#define VD_IMPLEMENT_ENTITY_AND_CREATE_FN(entity, name, parent) \
    Core::Entity* vdCreate ##entity () { \
        return VD_NEW(entity); \
    } \
    Core::Traits entity::GlobalTraits = Core::Traits(name, parent, false, (void *) &vdCreate ##entity, NULL); \
    const Core::Traits* entity::GetTraits() const { \
        return &GlobalTraits;\
    }

#define VD_IMPLEMENT_ENTITY_AND_LOAD_FN(entity, name, parent) \
    Core::Entity* vdLoad ##entity (Core::Stream* stream, Core::Registry* registry) { \
        return VD_NEW(entity, stream, registry); \
    } \
    Core::Traits entity::GlobalTraits = Core::Traits(name, parent, false, NULL, (void *) &vdLoad ##entity ); \
    const Core::Traits* entity::GetTraits() const { \
        return &GlobalTraits;\
    }

#define VD_IMPLEMENT_ENTITY_AND_CREATE_LOAD_FN(entity, name, parent) \
    Core::Entity* vdLoad ##entity (Core::Stream *stream, Core::Registry* registry) { \
        return VD_NEW(entity, stream, registry); \
    } \
    Core::Entity* vdCreate ##entity () { \
        return VD_NEW(entity); \
    } \
    Core::Traits entity::GlobalTraits = Core::Traits(name, parent, false, (void *) &vdCreate ##entity , (void *) &vdLoad ##entity ); \
    const Core::Traits* entity::GetTraits() const { \
        return &GlobalTraits;\
    }

// ============================================================================================== //

VD_CORE_NAMESPACE_END();

// ============================================================================================== //

#endif // VD_CORE_ENTITY_INCLUDED

