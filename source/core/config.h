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

#ifndef VD_CORE_CONFIG_INCLUDED
#define VD_CORE_CONFIG_INCLUDED

// ============================================================================================== //

#include "core/core.h"

#include <boost/variant.hpp>
#include <boost/unordered_map.hpp>

// ============================================================================================== //

VD_CORE_NAMESPACE_BEGIN();

// ============================================================================================== //

class VD_API Config 
{
public:

	// ------------------------------------------------------------------------------------------ //

	class VD_API Property
	{
	public:

		struct Data 
		{
			uint8_t* Ptr;
			size_t   Size;
		};

	    typedef boost::make_recursive_variant<bool,
	        vd::i8,  vd::u8,  vd::i16, vd::u16, 
	        vd::i32, vd::i32, vd::i64, vd::u64,
	        vd::f16, vd::f32, vd::f64, vd::string,
	        Property::Data, 
	        std::vector< boost::recursive_variant_ > >::type    ValueType;
	    typedef boost::unordered_map<vd::string, Property>      MapType;
	    typedef std::vector<Property>                           ListType;
	    typedef std::vector<vd::string>                         NameListType;
	    typedef std::vector<ValueType>                          ValueListType;
	    static const ValueType                                  InvalidValue;

	public:

		Property() : Flags(0) { }
		
		Property(const Property& other) 
			: Name(other.Name), Value(other.Value), Flags(other.Flags) { }

		Property(const vd::string& name, const ValueType& value) 
			: Name(name), Value(value), Flags(0) { }

	    vd::string    		  Name;
	    ValueType     		  Value;
	    vd::u32 			  Flags;
	};

	// ---------------------------------------------------------------------------------------------- //

	Config();
	Config(const vd::string &plugin_name);
	Config(const Property::ListType& properties);

	inline const vd::string& GetName() const { return m_Name; }
	inline void SetName(const vd::string& n) { m_Name = n; }

	inline void SetPluginName(const vd::string &name) { m_PluginName = name; }
	inline const vd::string& GetPluginName() const { return m_PluginName; }
	
    void
    Set(const vd::string& name, const Property::ValueType& value);
    
    const Property::ValueType& 
    Get(const vd::string& name, 
    	const Property::ValueType& missing = Property::InvalidValue) const;
    
    bool Remove(const vd::string& name);

    bool GetPropertyList(Property::ListType& result) const;
    bool GetPropertyNames(Property::NameListType& result) const;

    bool SetFlag(const vd::string& name, vd::u32 flag);
	bool IsMarked(const vd::string &name) const;
	bool Has(const vd::string &name) const;

	TypeId::Value GetTypeId(const vd::string &name) const;
	std::vector<vd::string> GetUnModified() const;
	vd::string ToString() const;

private:

    Property::MapType           m_Properties;
	vd::string 					m_PluginName;
	vd::string 					m_Name;
};

// ============================================================================================== //

VD_CORE_NAMESPACE_END();

// ============================================================================================== //

#endif // VD_CORE_CONFIG_INCLUDED

