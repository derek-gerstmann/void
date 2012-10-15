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

#include "core/config.h"
#include "core/logging.h"
#include "common/traits.h"
#include <boost/variant.hpp>

// ============================================================================================== //

VD_CORE_NAMESPACE_BEGIN();

// ============================================================================================== //

const Config::Property::ValueType Config::Property::InvalidValue = VD_U64_MAX; // (Constants::InvalidIndex);

// ============================================================================================== //

struct TypeIdVisitor : boost::static_visitor<TypeId::Value> 
{
    template <typename T>
    TypeId::Value operator()( const T & t ) const 				  	 		 
    { return TypeTraits<T>::GetTypeId(); }
    
    TypeId::Value operator()( const Config::Property::ValueType& v) const 	  
    { return TypeId::VAR; }
    
    TypeId::Value operator()( const std::vector<Config::Property::ValueType>& v) const 
    { return TypeId::ARR; }
};

// ============================================================================================== //

Config::Config() 
	: m_Name("Default") 
{ 
	// EMPTY!
}

Config::Config(
	const vd::string &plugin_name
) : 
	m_PluginName(plugin_name), 
	m_Name("Default") 
{ 
    // EMPTY!
}

Config::Config(
    const Config::Property::ListType& properties)
{
    for(size_t n = 0; n < properties.size(); n++)
    { 
        Set(properties[n].Name, properties[n].Value); 
    }
}

void 
Config::Set(
    const vd::string& name, 
    const Config::Property::ValueType& value)
{ 
    m_Properties[name] = Property(name, value); 
}

const Config::Property::ValueType& 
Config::Get(
    const vd::string& name, 
    const Config::Property::ValueType& missing) const
{
    Config::Property::MapType::const_iterator it;
    it = m_Properties.find(name);
    if(it != m_Properties.end())
        return it->second.Value;
    return missing;     
}

bool 
Config::Remove(
    const vd::string& name)
{
    Config::Property::MapType::const_iterator it;
    it = m_Properties.find(name);
    if(it != m_Properties.end())
    {
        m_Properties.erase(it->first);
        return true;
    }
    return false;
}

bool
Config::GetPropertyList(
    Config::Property::ListType& result) const
{
    if(m_Properties.size() < 1) 
        return false;

    result.clear();
    Config::Property::MapType::const_iterator it;
    for(it = m_Properties.begin(); it != m_Properties.end(); it++)
        result.push_back(it->second);

    return true;
}

bool
Config::GetPropertyNames(
    Config::Property::NameListType& result) const
{
    if(m_Properties.size() < 1) 
        return false;

    result.clear();
    Config::Property::MapType::const_iterator it;
    for(it = m_Properties.begin(); it != m_Properties.end(); it++)
        result.push_back(it->first);

    return true;
}

// ============================================================================================== //

bool Config::Has(
	const std::string &name) const 
{
	return m_Properties.find(name) != m_Properties.end();
}

std::vector<std::string> 
Config::GetUnModified() const 
{
	Config::Property::MapType::const_iterator it = m_Properties.begin();
	std::vector<std::string> result;
	for (; it != m_Properties.end(); ++it) 
	{
		if (!(*it).second.Flags)
			result.push_back((*it).first);
	}
	return result;
}

TypeId::Value
Config::GetTypeId(const std::string &name) const 
{
	Config::Property::MapType::const_iterator it = m_Properties.find(name);
	if (it == m_Properties.end())
		vdLogGlobalError("Property '%s' has not been specified!", name.c_str());
	
	TypeIdVisitor visitor;
	return boost::apply_visitor(visitor, it->second.Value);
}

std::string 
Config::ToString() const 
{
//	Config::Property::MapType::const_iterator it = m_Properties.begin();
	std::ostringstream oss;

	oss << "Config[" << std::endl
		<< "  PluginName = \"" << m_PluginName << "\"," << std::endl
		<< "  Entries = {" << std::endl;

#if 0	
	while (it != m_Properties.end()) 
	{
		oss << "    \"" << (*it).first << "\" -> ";
		const EntryData &data = (*it).second.Value;
		Config::PropertyType::Value type = boost::apply_visitor(type_visitor(), data);
		switch (type) 
		{
			case Config::PropertyType::Boolean:
				oss << (boost::get<bool>(data) ? "true" : "false");
				break;
			case Config::PropertyType::Integer:
				oss << boost::get<vd::i64>(data);
				break;
			case Config::PropertyType::Float:
				oss << boost::get<Float>(data);
				break;
			case Config::PropertyType::Point:
				oss << boost::get<Point>(data).toString();
				break;
//			case Config::PropertyType::Transform:
//				oss << boost::get<Transform>(data).toString();
//				break;
//			case Config::PropertyType::Spectrum:
//				oss << boost::get<Spectrum>(data).toString();
//				break;
			case Config::PropertyType::String:
				oss << "\"" << boost::get<std::string>(data) << "\"";
				break;
			case Config::PropertyType::Data:
				oss << boost::get<Data>(data).Ptr << " (size=" 
					<< boost::get<Data>(data).Size << ")";
				break;
			default:
				vdLogGlobalError("Encountered an unknown property type!");
		}
		if (++it != m_Properties.end())
			oss << ",";
		oss << endl;
	}
#endif
	oss << "  }" << std::endl
		<< "]" << std::endl;
	return oss.str();
}

bool Config::SetFlag(
	const std::string &name, vd::u32 flag) 
{
	Config::Property::MapType::iterator it = m_Properties.find(name);
	if (it == m_Properties.end())
		return false;
	it->second.Flags |= (1u << flag);
	return true;
}

bool Config::IsMarked(
	const std::string &name) const 
{
	Config::Property::MapType::const_iterator it = m_Properties.find(name);
	if (it == m_Properties.end())
		vdLogGlobalError("Could not find parameter '%s'!", name.c_str());
	return it->second.Flags != 0;
}

// ============================================================================================== //

VD_CORE_NAMESPACE_END();

// ============================================================================================== //
