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

#ifndef VD_CORE_OBJECT_INCLUDED
#define VD_CORE_OBJECT_INCLUDED

// ============================================================================================== //

#include "core/core.h"
#include "core/shared.h"
#include "core/atomics.h"
#include "core/metaclass.h"

// ============================================================================================== //

VD_CORE_NAMESPACE_BEGIN();

// ============================================================================================== //

class Repository;

// ============================================================================================== //

class Object : public Shared<Object>
{
public:
	
    Object(const Object* parent=NULL);

    virtual vd::status 
    Destroy(void);

	void SetId(vd::uid id);
	vd::uid GetId() const;
    
    virtual vd::string 
    ToString() const;

	static const char* 
	GetName();

	static const char* 
	GetIdentifier();

	virtual Core::Object* 
	Locate( const char* identifier ) const;
	
	virtual const 
	Core::MetaClass* GetMetaClass() const;
	
	static Core::MetaClass m_MetaClass;

protected:
    virtual ~Object();
    
protected:
	vd::uid m_Id;
};

// ============================================================================================== //

template< class T >
T* vd_cast( Core::Object* object )
{
	if ( !object ) return NULL;
	Core::Object* other = object->Locate( T::GetIdentifier() );
	if ( !other ) return NULL;

	return static_cast<T*>(other);
}

template< class T >
bool vd_isa( Core::Object* object )
{
    if ( object == NULL ) return false;
    Core::Object* other = object->Locate( T::GetIdentifier() );
    if ( other != NULL ) return true;

    return false;
}

// ============================================================================================== //
/// Object and Class Declaration Macros
// ============================================================================================== //

#define VD_META_CLASS(x) x::m_MetaClass

// ============================================================================================== //

#define VD_DECLARE_DYNAMIC_RESOLVE( ThisName, BaseName )					\
																			\
	static const char* GetIdentifier() const								\
	{																		\
		static const char* ident = #BaseName ":" #ThisName;					\
		return ident;														\
	};																		\
																			\
	virtual Object* Locate( const char* identifier ) const      			\
	{																		\
		if ( GetIdentifier() == identifier )								\
			return this;													\
																			\
		return BaseClass::Locate( identifier);								\
	}

// ============================================================================================== //

#define VD_DERIVE_OBJECT( ThisName, BaseName )								\
	public:																	\
		typedef BaseName BaseClass;											\
		typedef ThisName ThisClass;											\
		VD_DECLARE_DYNAMIC_RESOLVE( ThisName, BaseName )					\
		ThisName( Object* parent=NULL )

#define VD_DECLARE_DERIVED_OBJECT( ThisName, BaseName )						\
	VD_DERIVE_OBJECT( ThisName, BaseName ) : BaseClass( parent )

#define VD_OBJECT_CONSTRUCTOR( ThisName )									\
	ThisName::ThisName( Core::Object* parent ) : BaseClass( parent )

// ============================================================================================== //

#define VD_DECLARE_OBJECT( ThisName ) \
public: \
	virtual const Core::MetaClass* GetMetaClass() const; \
	static Core::MetaClass m_MetaClass;

#define VD_IMPLEMENT_ABSTRACT_OBJECT(object, name, parent) \
	Core::MetaClass object::m_MetaClass = Core::MetaClass(name, parent, true); \
	const Core::MetaClass* object::GetMetaClass() const { \
		return &m_MetaClass;\
	}

#define VD_IMPLEMENT_OBJECT(object, name, parent) \
	Core::MetaClass object::m_MetaClass = Core::MetaClass(name, parent, false); \
	const Core::MetaClass* object::GetMetaClass() const { \
		return &m_MetaClass;\
	}

#define VD_IMPLEMENT_OBJECT_AND_CREATE_FN(object, name, parent) \
	Core::Object* vdCreate ##object () { \
		return VD_NEW(object); \
	} \
	Core::MetaClass object::m_MetaClass = Core::MetaClass(name, parent, false, (void *) &vdCreate ##object, NULL); \
	const Core::MetaClass* object::GetMetaClass() const { \
		return &m_MetaClass;\
	}

#define VD_IMPLEMENT_OBJECT_AND_LOAD_FN(object, name, parent) \
	Core::Object* vdLoad ##object (Core::Stream* stream, Core::Repository* repo) { \
		return VD_NEW(object, stream, repo); \
	} \
	Core::MetaClass object::m_MetaClass = Core::MetaClass(name, parent, false, NULL, (void *) &vdLoad ##object ); \
	const Core::MetaClass* object::GetMetaClass() const { \
		return &m_MetaClass;\
	}

#define VD_IMPLEMENT_OBJECT_AND_CREATE_LOAD_FN(object, name, parent) \
	Core::Object* vdLoad ##object (Core::Stream *stream, Core::Repository *repo) { \
		return VD_NEW(object, stream, repo); \
	} \
	Core::Object* vdCreate ##object () { \
		return VD_NEW(object); \
	} \
	Core::MetaClass object::m_MetaClass = Core::MetaClass(name, parent, false, (void *) &vdCreate ##object , (void *) &vdLoad ##object ); \
	const Core::MetaClass* object::GetMetaClass() const { \
		return &m_MetaClass;\
	}

// ============================================================================================== //

VD_CORE_NAMESPACE_END();

// ============================================================================================== //

#endif // VD_CORE_OBJECT_INCLUDED

