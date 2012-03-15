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

#ifndef VD_CONTAINERS_PARAMSET_INCLUDED
#define VD_CONTAINERS_PARAMSET_INCLUDED

// ============================================================================================== //

#include "containers/containers.h"
#include "core/symbol.h"
#include "core/object.h"
#include "core/numerics.h"
#include "constants/constants.h"

// ============================================================================================== //

VD_CONTAINERS_NAMESPACE_BEGIN();

// ============================================================================================== //

enum ScopeType
{
	InvalidScope,
	GlobalScope,
	ApplicationScope,
	ProgramScope,
	GraphicsScope,
	ShaderScope,
	StorageScope,
	ScopeCount
};

enum AccessType
{
	InvalidAccess,
	PublicAccess,
	PrivateAccess,
	InternalAccess,
	ScopeAccess,
	AccessCount
};

// ============================================================================================== //

template< class Type >
class Parameter
{
public:

    Parameter() :
        Changed(false),
        Scope(InvalidScope),
        Access(InvalidAccess)
    {

    }

    Parameter(
    	const vd::symbol& n, 
    	const Type& v, 
    	const Type& i
    ) :
        Changed(true),
        Scope(GlobalScope),
        Access(PublicAccess),
        Name(n),
        Value(v),
        Initial(i)
    {

    }

    Parameter(
    	const vd::symbol& n, 
    	const Type& v,
    	AccessType access = PublicAccess,
    	ScopeType scope = GlobalScope
    ) :
        Changed(true),
        Scope(scope),
        Access(access),
        Name(n),
        Value(v),
        Initial(v)
    {

    }

    void Set(const Type& v)
    {
		Value = v;
		Changed = true;
    }
    
    const Type& Get() const
    {
        return Value;
    }
    
    void ClearChanges()
    {
        Changed = false;
    }

    void Reset()
    {
        Value = Initial;
        Changed = true;
    }
    
    const vd::symbol& GetName() const 
    { 
    	return Name; 
    }
    
public:

    bool Changed;
	ScopeType Scope;
	AccessType Access;
    vd::symbol Name;
    Type Value;
    Type Initial;
};

// ============================================================================================== //

class ParamSet : public Core::Object
{
public:

	enum Type
	{
		Invalid,
        Str,
		Flag,
		I32,
		F32,
		V2F32,
		V3F32,
		V4F32,
		M3F32,
		M4F32,
		Count,
	};

	struct EmptyType
	{
        VD_FORCE_INLINE operator vd::string( ) const   { return vd::string(); }
		VD_FORCE_INLINE operator flag( ) const 	{ return Constants::PosInf; }
		VD_FORCE_INLINE operator i32( ) const 	{ return Constants::PosInf; }
		VD_FORCE_INLINE operator f32( ) const 	{ return Constants::PosInf; }
		VD_FORCE_INLINE operator v2f32( ) const { return v2f32(Constants::PosInf); }
		VD_FORCE_INLINE operator v3f32( ) const { return v3f32(Constants::PosInf); }
		VD_FORCE_INLINE operator v4f32( ) const { return v4f32(Constants::PosInf); }
		VD_FORCE_INLINE operator m3f32( ) const { return m3f32(Constants::Zero); }
		VD_FORCE_INLINE operator m4f32( ) const { return m4f32(Constants::Zero); }
	} Empty;

    typedef AlignedMap< vd::uid, ParamSet::Type >::type 		MapTypes;
    typedef AlignedMap< vd::uid, Parameter<vd::string> >::type  MapStrings;
    typedef AlignedMap< vd::uid, Parameter<vd::flag> >::type 	MapFlags;
    typedef AlignedMap< vd::uid, Parameter<vd::i32> >::type 	MapI32;
    typedef AlignedMap< vd::uid, Parameter<vd::f32> >::type 	MapF32;
    typedef AlignedMap< vd::uid, Parameter<vd::v2f32> >::type   MapV2F32;
    typedef AlignedMap< vd::uid, Parameter<vd::v3f32> >::type   MapV3F32;
    typedef AlignedMap< vd::uid, Parameter<vd::v4f32> >::type   MapV4F32;
    typedef AlignedMap< vd::uid, Parameter<vd::m3f32> >::type   MapM3F32;
    typedef AlignedMap< vd::uid, Parameter<vd::m4f32> >::type   MapM4F32;
    typedef Vector< vd::uid >::type                             ParamVector;
	
public:

	ParamSet() : Core::Object() { }
	virtual ~ParamSet() { }

    bool AddStr(const vd::symbol& symbol, const vd::string& value, 
        AccessType access = PublicAccess, 
        ScopeType scope = GlobalScope);
    
    bool Add1b(const vd::symbol& symbol, vd::flag value, 
    	AccessType access = PublicAccess, 
    	ScopeType scope = GlobalScope);

    bool Add1i(const vd::symbol& symbol, vd::i32 value, 
    	AccessType access = PublicAccess, 
    	ScopeType scope = GlobalScope);
        
    bool Add1f(const vd::symbol& symbol, vd::f32 value, 
    	AccessType access = PublicAccess, 
    	ScopeType scope = GlobalScope);

    bool Add2f(const vd::symbol& symbol, const vd::v2f32& value, 
    	AccessType access = PublicAccess, 
    	ScopeType scope = GlobalScope);
    
    bool Add3f(const vd::symbol& symbol, const vd::v3f32& value, 
    	AccessType access = PublicAccess, 
    	ScopeType scope = GlobalScope);

    bool Add4f(const vd::symbol& symbol, const vd::v4f32& value, 
    	AccessType access = PublicAccess, 
    	ScopeType scope = GlobalScope);

    bool Add3mf(const vd::symbol& symbol, const vd::m3f32& value, 
    	AccessType access = PublicAccess, 
    	ScopeType scope = GlobalScope);    

    bool Add4mf(const vd::symbol& symbol, const vd::m4f32& value, 
    	AccessType access = PublicAccess, 
    	ScopeType scope = GlobalScope);

	bool IsA(Type t, const vd::symbol& name);
    bool IsUsed(const vd::symbol& name);
    bool IsChanged(const vd::symbol& name);
    bool ClearChanges(const vd::symbol& name);

    vd::string GetStr(const vd::symbol& name) const;
    vd::flag Get1b(const vd::symbol& name) const;
    vd::i32 Get1i(const vd::symbol& name) const;
    vd::f32 Get1f(const vd::symbol& name) const;
    const vd::v2f32& Get2f(const vd::symbol& name) const;
    const vd::v3f32& Get3f(const vd::symbol& name) const;
    const vd::v4f32& Get4f(const vd::symbol& name) const;
    const vd::m3f32& Get3mf(const vd::symbol& name) const;
    const vd::m4f32& Get4mf(const vd::symbol& name) const;

    vd::string GetStr(const vd::symbol& name, const vd::string& missing) const;
    vd::flag Get1b(const vd::symbol& name, vd::flag missing) const;
    vd::i32 Get1i(const vd::symbol& name, vd::i32 missing) const;
    vd::f32 Get1f(const vd::symbol& name, vd::f32 missing) const;
    const vd::v2f32& Get2f(const vd::symbol& name, const vd::v2f32& missing) const;
    const vd::v3f32& Get3f(const vd::symbol& name, const vd::v3f32& missing) const;
    const vd::v4f32& Get4f(const vd::symbol& name, const vd::v4f32& missing) const;
    const vd::m3f32& Get3mf(const vd::symbol& name, const vd::m3f32& missing) const;
    const vd::m4f32& Get4mf(const vd::symbol& name, const vd::m4f32& missing) const;

    bool SetStr(const vd::symbol& name, const vd::string& value);
    bool Set1i(const vd::symbol& name, vd::i32 value);
    bool Set1b(const vd::symbol& name, vd::flag value);
    bool Set1f(const vd::symbol& name, vd::f32 value);
    bool Set2f(const vd::symbol& name, const vd::v2f32& value);
    bool Set3f(const vd::symbol& name, const vd::v3f32& value);
    bool Set4f(const vd::symbol& name, const vd::v4f32& value);
    bool Set3mf(const vd::symbol& name, const vd::m3f32& value);
    bool Set4mf(const vd::symbol& name, const vd::m4f32& value);

    Parameter<vd::string>& GetParamStr(const vd::symbol& name);
    Parameter<vd::flag>& GetParam1b(const vd::symbol& name);
    Parameter<vd::i32>& GetParam1i(const vd::symbol& name);
    Parameter<vd::f32>& GetParam1f(const vd::symbol& name);
    Parameter<vd::v2f32>& GetParam2f(const vd::symbol& name);
    Parameter<vd::v3f32>& GetParam3f(const vd::symbol& name);
    Parameter<vd::v4f32>& GetParam4f(const vd::symbol& name);
    Parameter<vd::m3f32>& GetParam3mf(const vd::symbol& name);
    Parameter<vd::m4f32>& GetParam4mf(const vd::symbol& name);

    bool Remove(const vd::symbol& name);
    bool Reset(const vd::symbol& name);
    bool IsStale() const;
    void ClearAllChanges();
    vd::bytesize GetChanges(ParamVector& params) const;
    vd::bytesize GetAllSymbols(ParamVector& params) const;
    const vd::symbol& GetSymbol(vd::uid uuid) const;
    bool Save(const vd::string& filename) const;

	size_t Size() const { return m_Types.size(); }
	
    vd::string ToString() const;

	void Clear() 
	{
		m_Types.clear();
        m_Strings.clear();
		m_Flags.clear();
		m_i32.clear();
		m_f32.clear();
		m_v2f32.clear();
		m_v3f32.clear();
		m_v4f32.clear();
		m_m3f32.clear();
		m_m4f32.clear();
	}

	VD_DECLARE_OBJECT(ParamSet);

protected:

	VD_DISABLE_COPY_CONSTRUCTORS(ParamSet);

    MapTypes    m_Types;
    MapStrings  m_Strings;
    MapFlags    m_Flags;
    MapI32      m_i32;
    MapF32      m_f32;
    MapV2F32    m_v2f32;
    MapV3F32    m_v3f32;
    MapV4F32    m_v4f32;
    MapM3F32    m_m3f32;
    MapM4F32    m_m4f32;

};

// ============================================================================================== //

VD_CONTAINERS_NAMESPACE_END();

// ============================================================================================== //

#endif  // VD_CONTAINERS_PARAMSET_INCLUDED
