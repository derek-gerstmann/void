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

#include "containers/paramset.h"
#include "core/symbol.h"
#include "core/macros.h"
#include "core/logging.h"

// ============================================================================================== //

VD_CONTAINERS_NAMESPACE_BEGIN();

// ============================================================================================== //

bool
ParamSet::IsA(
	ParamSet::Type type, const vd::symbol& name)
{		
	if(m_Types.count(name.ToKey()) > 0)
		return m_Types[name.ToKey()] == type;
	return false;
}

bool
ParamSet::IsUsed(
	const vd::symbol& name)
{		
	return m_Types.count(name.ToKey()) > 0;
}

bool
ParamSet::IsChanged(
	const vd::symbol& name)
{
	if(IsUsed(name) == false)
		return false;
		
	ParamSet::Type vt = m_Types[name.ToKey()];
	switch(vt)
	{
	case ParamSet::Str:		{ return m_Strings[name.ToKey()].Changed; }
	case ParamSet::Flag:	{ return   m_Flags[name.ToKey()].Changed; }
	case ParamSet::I32:		{ return     m_i32[name.ToKey()].Changed; }
	case ParamSet::F32:		{ return     m_f32[name.ToKey()].Changed; }
	case ParamSet::V2F32:	{ return   m_v2f32[name.ToKey()].Changed; }
	case ParamSet::V3F32:	{ return   m_v3f32[name.ToKey()].Changed; }
	case ParamSet::V4F32:	{ return   m_v4f32[name.ToKey()].Changed; }
	case ParamSet::M3F32:	{ return   m_m3f32[name.ToKey()].Changed; }
	case ParamSet::M4F32:	{ return   m_m4f32[name.ToKey()].Changed; }
	default:				{ return false; }
	};
	return false;
}

bool
ParamSet::ClearChanges(
	const vd::symbol& name)
{
	if(m_Types.count(name.ToKey()) < 1)
		return false;
		
	ParamSet::Type vt = m_Types[name.ToKey()];
	switch(vt)
	{
	case ParamSet::Str:		{ m_Strings[name.ToKey()].ClearChanges(); break; }
	case ParamSet::Flag:	{   m_Flags[name.ToKey()].ClearChanges(); break; }
	case ParamSet::I32:		{     m_i32[name.ToKey()].ClearChanges(); break; }
	case ParamSet::F32:		{     m_f32[name.ToKey()].ClearChanges(); break; }
	case ParamSet::V2F32:	{   m_v2f32[name.ToKey()].ClearChanges(); break; }
	case ParamSet::V3F32:	{   m_v3f32[name.ToKey()].ClearChanges(); break; }
	case ParamSet::V4F32:	{   m_v4f32[name.ToKey()].ClearChanges(); break; }
	case ParamSet::M3F32:	{   m_m3f32[name.ToKey()].ClearChanges(); break; }
	case ParamSet::M4F32:	{   m_m4f32[name.ToKey()].ClearChanges(); break; }
	default:
		return false;
	};
	
	return true;
}

bool
ParamSet::AddStr(
	const vd::symbol& name, const vd::string& value,
	AccessType access, ScopeType scope)
{
	m_Types[name.ToKey()] = ParamSet::Flag;
    m_Strings[name.ToKey()] = Parameter<vd::string>(name, value, access, scope);
	return true;
}

bool
ParamSet::Add1b(
	const vd::symbol& name, vd::flag value,
	AccessType access, ScopeType scope)
{
	m_Types[name.ToKey()] = ParamSet::Flag;
    m_Flags[name.ToKey()] = Parameter<vd::flag>(name, value, access, scope);
	return true;
}

bool
ParamSet::Add1i(
	const vd::symbol& name, vd::i32 value,
	AccessType access, ScopeType scope)
{
	m_Types[name.ToKey()] = ParamSet::I32;
    m_i32[name.ToKey()] = Parameter<vd::i32>(name, value, access, scope);
	return true;
}

bool
ParamSet::Add1f(
	const vd::symbol& name, vd::f32 value, 
	AccessType access, ScopeType scope)
{
	m_Types[name.ToKey()] = ParamSet::F32;
    m_f32[name.ToKey()] = Parameter<vd::f32>(name, value, access, scope);
	return true;
}

bool
ParamSet::Add2f(
	const vd::symbol& name, const vd::v2f32& value,
	AccessType access, ScopeType scope)	
{
	m_Types[name.ToKey()] = ParamSet::V2F32;
    m_v2f32[name.ToKey()] = Parameter<vd::v2f32>(name, value, access, scope);
	return true;
}

bool
ParamSet::Add3f(
	const vd::symbol& name, const vd::v3f32& value,
	AccessType access, ScopeType scope)	
{
	m_Types[name.ToKey()] = ParamSet::V3F32;
    m_v3f32[name.ToKey()] = Parameter<vd::v3f32>(name, value, access, scope);
	return true;
}

bool
ParamSet::Add4f(
	const vd::symbol& name, const vd::v4f32& value,
	AccessType access, ScopeType scope)	
{
	m_Types[name.ToKey()] = ParamSet::V4F32;
    m_v4f32[name.ToKey()] = Parameter<vd::v4f32>(name, value, access, scope);
	return true;
}

bool
ParamSet::Add3mf(
	const vd::symbol& name, const vd::m3f32& value,
	AccessType access, ScopeType scope)	
{
	m_Types[name.ToKey()] = ParamSet::M3F32;
    m_m3f32[name.ToKey()] = Parameter<vd::m3f32>(name, value, access, scope);
	return true;
}

bool
ParamSet::Add4mf(
	const vd::symbol& name, const vd::m4f32& value,
	AccessType access, ScopeType scope)	
{
	m_Types[name.ToKey()] = ParamSet::M4F32;
    m_m4f32[name.ToKey()] = Parameter<vd::m4f32>(name, value, access, scope);
	return true;
}

// ============================================================================================== //

vd::string
ParamSet::GetStr(
	const vd::symbol& name) const
{
	return GetStr(name, ParamSet::Empty);
}

vd::flag
ParamSet::Get1b(
	const vd::symbol& name) const
{
	return Get1b(name, ParamSet::Empty);
}

vd::f32
ParamSet::Get1f(
	const vd::symbol& name) const
{
	return Get1f(name, ParamSet::Empty);
}

vd::i32
ParamSet::Get1i(
	const vd::symbol& name) const
{
	return Get1i(name, ParamSet::Empty);
}

const vd::v2f32&
ParamSet::Get2f(
	const vd::symbol& name) const
{
	return Get2f(name, ParamSet::Empty);
}

const vd::v3f32&
ParamSet::Get3f(
	const vd::symbol& name) const
{
	return Get3f(name, ParamSet::Empty);
}

const vd::v4f32&
ParamSet::Get4f(
	const vd::symbol& name) const
{
	return Get4f(name, ParamSet::Empty);
}

const vd::m3f32&
ParamSet::Get3mf(
	const vd::symbol& name) const
{
	return Get3mf(name, ParamSet::Empty);
}

const vd::m4f32&
ParamSet::Get4mf(
	const vd::symbol& name) const
{
	return Get4mf(name, ParamSet::Empty);
}

// ============================================================================================== //

vd::string
ParamSet::GetStr(
	const vd::symbol& name,
	const vd::string& missing) const
{
	MapStrings::const_iterator it;
	it = m_Strings.find(name.ToKey());
	if(it != m_Strings.end())
		return it->second.Value;
	return missing;		
}

vd::flag
ParamSet::Get1b(
	const vd::symbol& name,
	const vd::flag missing) const
{
	MapFlags::const_iterator it;
	it = m_Flags.find(name.ToKey());
	if(it != m_Flags.end())
		return it->second.Value;
	return missing;		
}

vd::f32
ParamSet::Get1f(
	const vd::symbol& name, 
	const vd::f32 missing) const
{
	MapF32::const_iterator it;
	it = m_f32.find(name.ToKey());
	if(it != m_f32.end())
		return it->second.Value;
	return missing;		
}

vd::i32
ParamSet::Get1i(
	const vd::symbol& name, 
	const vd::i32 missing) const
{
	MapI32::const_iterator it;
	it = m_i32.find(name.ToKey());
	if(it != m_i32.end())
		return it->second.Value;
	return missing;		
}

const vd::v2f32&
ParamSet::Get2f(
	const vd::symbol& name, 
	const vd::v2f32& missing) const
{
	MapV2F32::const_iterator it;
	it = m_v2f32.find(name.ToKey());
	if(it != m_v2f32.end())
		return it->second.Value;
	return missing;		
}

const vd::v3f32&
ParamSet::Get3f(
	const vd::symbol& name, 
	const vd::v3f32& missing) const
{
	MapV3F32::const_iterator it;
	it = m_v3f32.find(name.ToKey());
	if(it != m_v3f32.end())
		return it->second.Value;
	return missing;		
}

const vd::v4f32&
ParamSet::Get4f(
	const vd::symbol& name, 
	const vd::v4f32& missing) const
{
	MapV4F32::const_iterator it;
	it = m_v4f32.find(name.ToKey());
	if(it != m_v4f32.end())
		return it->second.Value;
	return missing;		
}

const vd::m3f32&
ParamSet::Get3mf(
	const vd::symbol& name, 
	const vd::m3f32& missing) const
{
	MapM3F32::const_iterator it;
	it = m_m3f32.find(name.ToKey());
	if(it != m_m3f32.end())
		return it->second.Value;
	return missing;		
}

const vd::m4f32&
ParamSet::Get4mf(
	const vd::symbol& name, 
	const vd::m4f32& missing) const
{
	MapM4F32::const_iterator it;
	it = m_m4f32.find(name.ToKey());
	if(it != m_m4f32.end())
		return it->second.Value;
	return missing;		
}

// ============================================================================================== //

bool
ParamSet::SetStr(
	const vd::symbol& name, 
	const vd::string& value)
{
	if(!IsA(ParamSet::Str, name))
		return AddStr(name, value);

	m_Strings[name.ToKey()].Set(value);
    return true;
}

bool
ParamSet::Set1b(
	const vd::symbol& name, 
	vd::flag value)
{
	if(!IsA(ParamSet::Flag, name))
		return Add1b(name, value);
	
	m_Flags[name.ToKey()].Set(value);
    return true;
}

bool
ParamSet::Set1i(
	const vd::symbol& name, 
	vd::i32 value)
{
	if(!IsA(ParamSet::I32, name))
		return Add1i(name, value);
	
	if(m_i32[name.ToKey()].Value != value)
		m_i32[name.ToKey()].Set(value);
    
    return true;
}

bool
ParamSet::Set1f(
	const vd::symbol& name, 
	vd::f32 value)
{
	if(!IsA(ParamSet::F32, name))
		return Add1f(name, value);

	m_f32[name.ToKey()].Set(value);
    return true;
}

bool
ParamSet::Set2f(
	const vd::symbol& name, 
	const vd::v2f32& value)
{
	if(!IsA(ParamSet::V2F32, name))
		return Add2f(name, value);

	const vd::v2f32& a = m_v2f32[name.ToKey()].Value;
	if(a != value)
		m_v2f32[name.ToKey()].Set(value);

    return true;
}

bool
ParamSet::Set3f(
	const vd::symbol& name, 
	const vd::v3f32& value)
{
	if(!IsA(ParamSet::V3F32, name))
		return Add3f(name, value);

	const vd::v3f32& a = m_v3f32[name.ToKey()].Value;
	if(a != value)
		m_v3f32[name.ToKey()].Set(value);

    return true;
}

bool
ParamSet::Set4f(
	const vd::symbol& name, 
	const vd::v4f32& value)
{
	if(!IsA(ParamSet::V4F32, name))
		return Add4f(name, value);

	const vd::v4f32& a = m_v4f32[name.ToKey()].Value;

	if(a != value)
		m_v4f32[name.ToKey()].Set(value);

    return true;
}

bool
ParamSet::Set3mf(
	const vd::symbol& name, 
	const vd::m3f32& value)
{
	if(!IsA(ParamSet::M3F32, name))
		return Add3mf(name, value);
	
	m_m3f32[name.ToKey()].Set(value);
    return true;
}

bool
ParamSet::Set4mf(
	const vd::symbol& name, 
	const vd::m4f32& value)
{
	if(!IsA(ParamSet::M4F32, name))
		return Add4mf(name, value);
	
	m_m4f32[name.ToKey()].Set(value);
    return true;
}

// ============================================================================================== //

Parameter<vd::string>&
ParamSet::GetParamStr(const vd::symbol& name)
{
    return m_Strings[name.ToKey()];
}

Parameter<vd::flag>&
ParamSet::GetParam1b(const vd::symbol& name)
{
    return m_Flags[name.ToKey()];
}

Parameter<vd::i32>&
ParamSet::GetParam1i(const vd::symbol& name)
{
    return m_i32[name.ToKey()];
}

Parameter<vd::f32>&
ParamSet::GetParam1f(const vd::symbol& name)
{
    return m_f32[name.ToKey()];
}

Parameter<vd::v2f32>&
ParamSet::GetParam2f(const vd::symbol& name)
{
    return m_v2f32[name.ToKey()];
}

Parameter<vd::v3f32>&
ParamSet::GetParam3f(const vd::symbol& name)
{
    return m_v3f32[name.ToKey()];
}

Parameter<vd::v4f32>&
ParamSet::GetParam4f(const vd::symbol& name)
{
    return m_v4f32[name.ToKey()];
}

Parameter<vd::m3f32>&
ParamSet::GetParam3mf(const vd::symbol& name)
{
    return m_m3f32[name.ToKey()];
}

Parameter<vd::m4f32>&
ParamSet::GetParam4mf(const vd::symbol& name)
{
    return m_m4f32[name.ToKey()];
}

bool
ParamSet::Remove(const vd::symbol& name)
{
	if(m_Types.count(name.ToKey()) < 1)
		return false;
		
	ParamSet::Type vt = m_Types[name.ToKey()];
	switch(vt)
	{
	case ParamSet::Str:	
	{	
		m_Types.erase(name.ToKey()); 
		m_Strings.erase(name.ToKey()); 
		return true; 
	}
	case ParamSet::Flag:	
	{	
		m_Types.erase(name.ToKey()); 
		m_Flags.erase(name.ToKey()); 
		return true; 
	}
	case ParamSet::I32:
	{	
		m_Types.erase(name.ToKey());
	    m_i32.erase(name.ToKey());
		return true;
	}
	case ParamSet::F32:
	{	
		m_Types.erase(name.ToKey());
	    m_f32.erase(name.ToKey());
		return true;
	}
	case ParamSet::V2F32:
	{	
		m_Types.erase(name.ToKey());
	    m_v2f32.erase(name.ToKey());
		return true;
	}
	case ParamSet::V3F32:
	{	
		m_Types.erase(name.ToKey());
	    m_v3f32.erase(name.ToKey());
		return true;
	}
	case ParamSet::V4F32:
	{	
		m_Types.erase(name.ToKey());
	    m_v4f32.erase(name.ToKey());
		return true;
	}
	case ParamSet::M3F32:
	{	
		m_Types.erase(name.ToKey());
	    m_m3f32.erase(name.ToKey());
		return true;
	}
	case ParamSet::M4F32:
	{	
		m_Types.erase(name.ToKey());
	    m_m4f32.erase(name.ToKey());
		return true;
	}
	default:
		return false;
	};

	return false;
}

bool
ParamSet::Reset(const vd::symbol& name)
{
	if(m_Types.count(name.ToKey()) < 1)
		return false;
		
	ParamSet::Type vt = m_Types[name.ToKey()];
	switch(vt)
	{
	case ParamSet::Str:
	    m_Strings[name.ToKey()].Reset();
		return true;
	case ParamSet::Flag:
	    m_Flags[name.ToKey()].Reset();
		return true;
	case ParamSet::I32:
	    m_i32[name.ToKey()].Reset();
		return true;
	case ParamSet::F32:
	    m_f32[name.ToKey()].Reset();
		return true;
	case ParamSet::V2F32:
	    m_v2f32[name.ToKey()].Reset();
		return true;
	case ParamSet::V3F32:
	    m_v3f32[name.ToKey()].Reset();
		return true;
	case ParamSet::V4F32:
	    m_v4f32[name.ToKey()].Reset();
		return true;
	case ParamSet::M3F32:
	    m_m3f32[name.ToKey()].Reset();
		return true;
	case ParamSet::M4F32:
	    m_m4f32[name.ToKey()].Reset();
		return true;
	default:
		return false;
	};

	return false;
}

vd::bytesize
ParamSet::GetAllSymbols(
	ParamVector& params) const
{
	vd::bytesize count = 0;
	MapTypes::const_iterator it;
	for(it = m_Types.begin(); it != m_Types.end(); it++)
	{
		params.push_back(it->first);
		count++;
	}
	return count;
}

vd::bytesize
ParamSet::GetChanges(
	ParamVector& params) const
{
	{
		MapStrings::const_iterator it;
		for(it = m_Strings.begin(); it != m_Strings.end(); it++)
		{
			if(it->second.Changed)
				params.push_back(it->first);
		}
	}
	{
		MapFlags::const_iterator it;
		for(it = m_Flags.begin(); it != m_Flags.end(); it++)
		{
			if(it->second.Changed)
				params.push_back(it->first);
		}
	}
	{
		MapI32::const_iterator it;
		for(it = m_i32.begin(); it != m_i32.end(); it++)
		{
			if(it->second.Changed)
				params.push_back(it->first);
		}
	}
	{
		MapF32::const_iterator it;
		for(it = m_f32.begin(); it != m_f32.end(); it++)
		{
			if(it->second.Changed)
				params.push_back(it->first);
		}
	}
	{
		MapV2F32::const_iterator it;
		for(it = m_v2f32.begin(); it != m_v2f32.end(); it++)
		{
			if(it->second.Changed)
				params.push_back(it->first);
		}
	}
	{
		MapV3F32::const_iterator it;
		for(it = m_v3f32.begin(); it != m_v3f32.end(); it++)
		{
			if(it->second.Changed)
				params.push_back(it->first);
		}
	}
	{
		MapV4F32::const_iterator it;
		for(it = m_v4f32.begin(); it != m_v4f32.end(); it++)
		{
			if(it->second.Changed)
				params.push_back(it->first);
		}
	}
	{
		MapM3F32::const_iterator it;
		for(it = m_m3f32.begin(); it != m_m3f32.end(); it++)
		{
			if(it->second.Changed)
				params.push_back(it->first);
		}
	}

	{
		MapM4F32::const_iterator it;
		for(it = m_m4f32.begin(); it != m_m4f32.end(); it++)
		{
			if(it->second.Changed)
				params.push_back(it->first);
		}
	}
    return params.size();
}

bool
ParamSet::IsStale() const
{
	ParamSet::ParamVector changes;
    return GetChanges(changes) > 0;
}

void
ParamSet::ClearAllChanges()
{
	{
		MapStrings::iterator it;
		for(it = m_Strings.begin(); it != m_Strings.end(); it++)
		{
			it->second.Changed = false;
		}
	}
	{
		MapFlags::iterator it;
		for(it = m_Flags.begin(); it != m_Flags.end(); it++)
		{
			it->second.Changed = false;
		}
	}
	{
		MapI32::iterator it;
		for(it = m_i32.begin(); it != m_i32.end(); it++)
		{
			it->second.Changed = false;
		}
	}
	{
		MapF32::iterator it;
		for(it = m_f32.begin(); it != m_f32.end(); it++)
		{
			it->second.Changed = false;
		}
	}
	{
		MapV2F32::iterator it;
		for(it = m_v2f32.begin(); it != m_v2f32.end(); it++)
		{
			it->second.Changed = false;
		}
	}
	{
		MapV3F32::iterator it;
		for(it = m_v3f32.begin(); it != m_v3f32.end(); it++)
		{
			it->second.Changed = false;
		}
	}
	{
		MapV4F32::iterator it;
		for(it = m_v4f32.begin(); it != m_v4f32.end(); it++)
		{
			it->second.Changed = false;
		}
	}
	{
		MapM3F32::iterator it;
		for(it = m_m3f32.begin(); it != m_m3f32.end(); it++)
		{
			it->second.Changed = false;
		}
	}
	{
		MapM4F32::iterator it;
		for(it = m_m4f32.begin(); it != m_m4f32.end(); it++)
		{
			it->second.Changed = false;
		}
	}
}

vd::string
ParamSet::ToString() const
{
	std::stringstream ss;
    MapFlags::const_iterator blit;
    for(blit = m_Flags.begin(); blit != m_Flags.end(); blit++)
    {
        const Parameter<vd::flag>& p = blit->second;
        ss << p.Name.ToString() << "[ " << p.Value << "] " << std::endl;
    }

    MapI32::const_iterator iit;
    for(iit = m_i32.begin(); iit != m_i32.end(); iit++)
    {
        const Parameter<vd::i32>& p = iit->second;
        ss << p.Name.ToString() << "[ " << p.Value << "] " << std::endl;
    }

    MapF32::const_iterator fpit;
    for(fpit = m_f32.begin(); fpit != m_f32.end(); fpit++)
    {
        const Parameter<vd::f32>& p = fpit->second;
        ss << p.Name.ToString() << "[ " << p.Value << "] " << std::endl;
    }

	return ss.str();
}


bool
ParamSet::Save(const vd::string& filename) const
{
	return false;
/*
    std::ofstream file(filename.c_str());

    typedef msgpack::type::tuple<int, vd::string, vd::f32, vd::f32> FloatParamMsg;
    typedef msgpack::type::tuple<int, vd::string, int, int> IntegerParamMsg;
    typedef msgpack::type::tuple<int, vd::string, bool, bool> FlagParamMsg;
    typedef msgpack::type::tuple<int, vd::string, std::vector<vd::f32>, std::vector<vd::f32> > PointParamMsg;
    typedef msgpack::type::tuple<int, vd::string, std::vector<vd::f32>, std::vector<vd::f32> > MatrixParamMsg;

    msgpack::sbuffer buffer;
    msgpack::packer<msgpack::sbuffer> packer(&buffer);

    std::map< FloatParam, Parameter<vd::f32> >::iterator fpit;
    for(fpit = m_f32.begin(); fpit != m_f32.end(); fpit++)
    {
        Parameter<vd::f32>& p = fpit->second;
        FloatParamMsg msg(fpit->first, p.Name, p.Value, p.Initial);
        packer.pack(msg);

        vdInfo("FloatParam[%s] = %f\n", p.Name.c_str(), p.Value);
    }

    std::map< FlagParam, Parameter<vd::flag> >::iterator blit;
    for(blit = m_Flags.begin(); blit != m_Flags.end(); blit++)
    {
        Parameter<vd::flag>& p = blit->second;
        FlagParamMsg msg(fpit->first, p.Name, p.Value, p.Initial);
        packer.pack(msg);

        vdInfo("FlagParam[%s] = %s\n", p.Name.c_str(), p.Value ? "true" : "false");
    }

    std::map< IntegerParam, Parameter<int> >::iterator iit;
    for(iit = m_i32.begin(); iit != m_i32.end(); iit++)
    {
        Parameter<int>& p = iit->second;
        IntegerParamMsg msg(fpit->first, p.Name, p.Value, p.Initial);
        packer.pack(msg);

        vdInfo("IntParam[%s] = %d\n", p.Name.c_str(),  p.Value);
    }

    std::map< PointParam, Parameter<Point3> >::iterator pit;
    for(pit = p3s_.begin(); pit != p3s_.end(); pit++)
    {
        std::vector<vd::f32> value;
        std::vector<vd::f32> initial;

        Parameter<Point3>& p = pit->second;

        value.push_back(p.Value.getX());
        value.push_back(p.Value.getY());
        value.push_back(p.Value.getZ());

        initial.push_back(p.Initial.getX());
        initial.push_back(p.Initial.getY());
        initial.push_back(p.Initial.getZ());

        PointParamMsg msg(fpit->first, p.Name, value, initial);
        packer.pack(msg);

        const Point3& p0 = p.Value;

        vdInfo("Point[%s] = [%f %f %f]'\n",
               p.Name.c_str(),
               p0.getX(), p0.getY(), p0.getZ() );
    }

    std::map< MatrixParam, Parameter<Matrix4> >::iterator mxit;
    for(mxit = m_m4f32.begin(); mxit != m_m4f32.end(); mxit++)
    {
        Parameter<Matrix4>& p = mxit->second;
        std::vector<vd::f32> value;
        std::vector<vd::f32> initial;

        const vd::numerics::Vector4& r0 = p.Value.getRow(0);
        const vd::numerics::Vector4& r1 = p.Value.getRow(1);
        const vd::numerics::Vector4& r2 = p.Value.getRow(2);
        const vd::numerics::Vector4& r3 = p.Value.getRow(3);

        value.push_back( r0.getX() );
        value.push_back( r0.getY() );
        value.push_back( r0.getZ() );
        value.push_back( r0.getW() );

        value.push_back( r1.getX() );
        value.push_back( r1.getY() );
        value.push_back( r1.getZ() );
        value.push_back( r1.getW() );

        value.push_back( r2.getX() );
        value.push_back( r2.getY() );
        value.push_back( r2.getZ() );
        value.push_back( r2.getW() );

        value.push_back( r3.getX() );
        value.push_back( r3.getY() );
        value.push_back( r3.getZ() );
        value.push_back( r3.getW() );

        const vd::numerics::Vector4& i0 = p.Initial.getRow(0);
        const vd::numerics::Vector4& i1 = p.Initial.getRow(1);
        const vd::numerics::Vector4& i2 = p.Initial.getRow(2);
        const vd::numerics::Vector4& i3 = p.Initial.getRow(3);

        initial.push_back( i0.getX() );
        initial.push_back( i0.getY() );
        initial.push_back( i0.getZ() );
        initial.push_back( i0.getW() );

        initial.push_back( i1.getX() );
        initial.push_back( i1.getY() );
        initial.push_back( i1.getZ() );
        initial.push_back( i1.getW() );

        initial.push_back( i2.getX() );
        initial.push_back( i2.getY() );
        initial.push_back( i2.getZ() );
        initial.push_back( i2.getW() );

        initial.push_back( i3.getX() );
        initial.push_back( i3.getY() );
        initial.push_back( i3.getZ() );
        initial.push_back( i3.getW() );

        vdInfo("Matrix[%s] = [%f %f %f %f][%f %f %f %f][%f %f %f %f][%f %f %f %f]'\n",
               p.Name.c_str(),
               r0.getX(), r0.getY(), r0.getZ(), r0.getW(),
               r1.getX(), r1.getY(), r1.getZ(), r1.getW(),
               r2.getX(), r2.getY(), r2.getZ(), r2.getW(),
               r3.getX(), r3.getY(), r3.getZ(), r3.getW() );

        MatrixParamMsg msg(fpit->first, p.Name, value, initial);
        packer.pack(msg);
    }

    file.close();

    vdInfo("Saved Parameters to '%s'!\n", filename.c_str());
	*/
}

// ============================================================================================== //

VD_IMPLEMENT_OBJECT(ParamSet, vd_sym(ParamSet), vd_sym(Object));

// ============================================================================================== //

VD_CONTAINERS_NAMESPACE_END();

// ============================================================================================== //
