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

#ifndef VD_SCHEMA_DATA_MODEL_INCLUDED
#define VD_SCHEMA_DATA_MODEL_INCLUDED

// ============================================================================================== //

#include "schema/schema.h"
#include "schema/data/data.h"

#include <boost/variant.hpp>
#include <boost/unordered_map.hpp>

// ============================================================================================== //

VD_SCHEMA_NAMESPACE_BEGIN();

// ============================================================================================== //

namespace Data {
namespace Model {

// ============================================================================================== //

class Property
{
public:
    typedef boost::make_recursive_variant<bool,
        vd::i8,  vd::u8,  vd::i16, vd::u16, 
        vd::i32, vd::i32, vd::i64, vd::u64,
        vd::f16, vd::f32, vd::f64, vd::string, 
        std::vector< boost::recursive_variant_ > >::type    ValueType;
    typedef boost::unordered_map<vd::string, Property>      MapType;
    typedef std::vector<Property>                           ListType;
    typedef std::vector<vd::string>                         NameListType;
    typedef std::vector<ValueType>                          ValueListType;
    static const ValueType                                  InvalidValue;

public:

    Property();
    Property(const Property& other);
    Property(const vd::string& n, const ValueType& v); 

    const vd::string& GetName() const;
    const ValueType& GetValue() const;

protected:
    vd::string    m_Name;
    ValueType     m_Value;
};

// ---------------------------------------------------------------------------------------------- //

class Base 
{
public:
    Base();
    Base(const Base& other);
    Base(const Property::ListType& properties);

    void 
    Set(const vd::string& name, const Property::ValueType& value);

    const Property::ValueType& 
    Get(const vd::string& name, const Property::ValueType& missing = Property::InvalidValue) const;
    
    bool 
    Remove(const vd::string& name);

    bool GetPropertyList(Property::ListType& result) const;
    bool GetPropertyNames(Property::NameListType& result) const;

    MetaKey GetMetaKey() const;

protected:
    MetaKey                     m_MetaKey;
    Property::MapType           m_Properties;
};

// ---------------------------------------------------------------------------------------------- //

class Meta : public Base
{
public:
    Meta();
    Meta(const Base& other);
    Meta(const Property::ListType& properties);
};

// ============================================================================================== //

} // end namespace: Model
} // end namespace: Data

// ============================================================================================== //

VD_SCHEMA_NAMESPACE_END();

// ============================================================================================== //

#endif  // VD_SCHEMA_DATA_MODEL_INCLUDED

