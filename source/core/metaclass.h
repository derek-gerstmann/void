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

#ifndef VD_CORE_METACLASS_INCLUDED
#define VD_CORE_METACLASS_INCLUDED

// ============================================================================================== //

#include "core/core.h"
#include "core/symbol.h"

// ============================================================================================== //

VD_CORE_NAMESPACE_BEGIN();

// ============================================================================================== //

class Object;
class Symbol;
class Stream;
class Repository;

// ============================================================================================== //

class VD_API MetaClass
{
public:
    MetaClass(
		const vd::symbol& name, 
		const vd::symbol& parent,
		bool is_abstract = true, 
		void* createfn = NULL, 
		void* loadfn = NULL);

    inline const vd::symbol& GetClassName() const { return m_Name; }
    inline const vd::symbol& GetIdentifier() const { return m_Name; }
    inline bool IsAbstract() const { return m_IsAbstract; }
    inline bool IsInstantiable() const { return m_CreateFn != NULL; }
    inline bool IsSerializable() const { return m_LoadFn != NULL; }
    inline const MetaClass* GetParentClass() const { return m_ParentClass; }
    bool IsDerivedFrom(const MetaClass* metaclass) const;

    Object* Load(Stream* stream = NULL, Repository* repo = NULL) const;
    Object* Create() const;

    static void CreateRegistry();
    static void DestroyRegistry();
    static const MetaClass* Retrieve(const vd::symbol& name);
    static const MetaClass* Retrieve(const Core::Symbol& name);
    static void Register(MetaClass* metaclass);
    static inline bool HasRegistry() { return m_IsInitialized; }

private:
	void RegisterSelf(void);
	MetaClass* m_Next;
	
   	vd::symbol m_Name;
    vd::symbol m_ParentName;
    MetaClass* m_ParentClass;
    bool m_IsAbstract;
    void* m_CreateFn;
    void* m_LoadFn;
    static bool m_IsInitialized;
};

// ============================================================================================== //

VD_CORE_NAMESPACE_END();

// ============================================================================================== //

#endif // VD_CORE_METACLASS_INCLUDED

