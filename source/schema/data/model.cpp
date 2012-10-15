// ============================================================================================== //
//
// License:   The Lesser GNU Public License (LGPL) v3.0.
//
// Author(s):   Derek Gerstmann. The University of Western Australia (UWA).
//        As well as the shoulders of many giants...
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

#include "schema/data/model.h"
#include "constants/values.h"

#include <boost/variant.hpp>

// ============================================================================================== //

VD_SCHEMA_NAMESPACE_BEGIN();

// ============================================================================================== //

namespace Data {
namespace Model {

// ============================================================================================== //

const Property::ValueType Property::InvalidValue = VD_U64_MAX;

// ============================================================================================== //

Property::Property() 
{
    // EMPTY!
}

Property::Property(
    const Property& other
) :
    m_Name(other.m_Name),
    m_Value(other.m_Value)
{
    // EMPTY!
}

Property::Property(
    const vd::string& name, 
    const ValueType& value
) :     
    m_Name(name), 
    m_Value(value) 
{
    // EMPTY!
}

const vd::string&
Property::GetName() const
{
    return m_Name;
}

const Property::ValueType&
Property::GetValue() const
{
    return m_Value;
}

// ============================================================================================== //

Base::Base()
{
    // EMPTY!
}

Base::Base(
    const Base& other
) :
    m_MetaKey(other.m_MetaKey),
    m_Properties(other.m_Properties) 
{
    // EMPTY!
}

Base::Base(
    const Property::ListType& properties)
{
    for(size_t n = 0; n < properties.size(); n++)
    { 
        Set(properties[n].GetName(), properties[n].GetValue()); 
    }
}

void 
Base::Set(
    const vd::string& name, 
    const Property::ValueType& value)
{ 
    m_Properties[name] = Property(name, value); 
}

const Property::ValueType& 
Base::Get(
    const vd::string& name, 
    const Property::ValueType& missing) const
{
    Property::MapType::const_iterator it;
    it = m_Properties.find(name);
    if(it != m_Properties.end())
        return it->second.GetValue();
    return missing;     
}

bool 
Base::Remove(
    const vd::string& name)
{
    Property::MapType::const_iterator it;
    it = m_Properties.find(name);
    if(it != m_Properties.end())
    {
        m_Properties.erase(it->first);
        return true;
    }
    return false;
}

bool
Base::GetPropertyList(
    Property::ListType& result) const
{
    if(m_Properties.size() < 1) 
        return false;

    result.clear();
    Property::MapType::const_iterator it;
    for(it = m_Properties.begin(); it != m_Properties.end(); it++)
        result.push_back(it->second);

    return true;
}

bool
Base::GetPropertyNames(
    Property::NameListType& result) const
{
    if(m_Properties.size() < 1) 
        return false;

    result.clear();
    Property::MapType::const_iterator it;
    for(it = m_Properties.begin(); it != m_Properties.end(); it++)
        result.push_back(it->first);

    return true;
}

MetaKey
Base::GetMetaKey() const
{
    return m_MetaKey;
}

// ============================================================================================== //

Meta::Meta() : Base::Base()
{
    m_MetaKey.Type = Data::ModelType::MetaType;
}

Meta::Meta(
    const Base& other
) :
    Base::Base(other)
{
    m_MetaKey.Type = Data::ModelType::MetaType;
}

Meta::Meta(
    const Property::ListType& properties
) :
    Base::Base(properties)
{
    m_MetaKey.Type =  Data::ModelType::MetaType;
}

// ============================================================================================== //

} // end namespace: Model
} // end namespace: Data

// ============================================================================================== //

VD_SCHEMA_NAMESPACE_END();

// ============================================================================================== //
