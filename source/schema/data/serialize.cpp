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

#include "schema/graph/serialize.h"
#include "schema/data/model.h"

#include <boost/variant.hpp>
#include <boost/serialization/variant.hpp>

// ============================================================================================== //

VD_SCHEMA_NAMESPACE_BEGIN();

// ============================================================================================== //

namespace Serialize {

// ============================================================================================== //

template<class Archive>
struct ToJsonVisitor : 
    boost::static_visitor<bool> 
{
public:
    ToJsonVisitor(Archive& result) : Result(result) {}

    template <typename T>
    bool operator()( const T & t ) const 
    {
        Result << t;
        return true;
    }

    bool operator()( const std::string & t ) const 
    {
        static const std::string open_quote("\"");
        static const std::string close_quote("\"");

        size_t length = t.length();    
        if(length < 1) return false;

        bool is_unquoted = (t[0] != '"'        && 
                            t[0] != '\''       && 
                            t[length-1] != '"' && 
                            t[length-1] != '\'');

        if(is_unquoted)
        {
            Result << open_quote;
            Result << t;
            Result << close_quote;
        }
        else
        {
            Result << t;            
        }

        return true;
    }

    bool operator()(const float& v) const
    {
        std::stringstream ss;
        ss.precision(FLT_DIG);
        ss << std::fixed << v;
        Result << ss.str();
        return true;
    }

    bool operator()(const double& v) const
    {
        std::stringstream ss;
        ss.precision(DBL_DIG);
        ss << std::fixed << v;
        Result << ss.str();
        return true;
    }

    bool operator()(const Data::Model::Property::ValueType& v) const
    {
        static const std::string open_child("[ ");
        static const std::string close_child("] ");

        Result << open_child;
        boost::apply_visitor( ToJsonVisitor<Archive>(Result), v );
        Result << close_child;

        return true;
    }

    bool operator()(const std::vector<Data::Model::Property::ValueType>& v) const
    {
        static const std::string open_list("[ ");
        static const std::string close_list("] ");
        static const std::string next_list(", ");
        static const std::string end_list(" ");
           
        size_t size = v.size();
        if(size > 0)
        {
            Result << open_list;

            for(size_t n = 0; n < size; n++)
            {
               bool last_item = (n < size - 1);
               boost::apply_visitor( ToJsonVisitor<Archive>(Result), v[n] );
               Result << (last_item ? next_list : end_list);
            }

            Result << close_list;
        }

        return true;
    }

    Archive& Result;
};

// ============================================================================================== //

bool ToJson(
    std::ostream& output,
    const Data::Model::Property& property)
{
    static const std::string define_prop(" : ");

    output << "\"" << property.GetName() << "\"" << define_prop;
    boost::apply_visitor( ToJsonVisitor<std::ostream>(output), property.GetValue() );
    return true;
}

// ============================================================================================== //

} // end namespace: Serialize

// ============================================================================================== //

VD_SCHEMA_NAMESPACE_END();

// ============================================================================================== //
