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

#include "dmpg.h"
#include <boost/variant.hpp>
#include <boost/serialization/variant.hpp>

// ============================================================================================== //

namespace Data {

// ============================================================================================== //

const uint64_t Constants::InvalidMetaKeyId = uint64_t(0xFFFFFFFFFFFFFFFFULL);

// ============================================================================================== //

MetaKey::MetaKey() :
    Id(Constants::InvalidMetaKeyId),
    Type(VD_DATA_MODEL_TYPE_INVALID)
{
    // EMPTY!
}

bool
MetaKey::IsValid(MetaKey mk)
{
    if(mk.Id == Constants::InvalidMetaKeyId)
        return false;
    else if(mk.Type == VD_DATA_MODEL_TYPE_INVALID)
        return false;
    else if(mk.Type == VD_DATA_MODEL_TYPE_LAST_ID)
        return false;
    return true;
}

// ============================================================================================== //

namespace Model {

// ============================================================================================== //

const Property::ValueType Property::InvalidValue = VD_INVALID_INDEX;

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
    const std::string& name, 
    const ValueType& value
) :     
    m_Name(name), 
    m_Value(value) 
{
    // EMPTY!
}

const std::string&
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
    const std::string& name, 
    const Property::ValueType& value)
{ 
    m_Properties[name] = Property(name, value); 
}

const Property::ValueType& 
Base::Get(
    const std::string& name, 
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
    const std::string& name)
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
    m_MetaKey.Type = VD_DATA_MODEL_TYPE_META;
}

Meta::Meta(
    const Base& other
) :
    Base::Base(other)
{
    m_MetaKey.Type = VD_DATA_MODEL_TYPE_META;
}

Meta::Meta(
    const Property::ListType& properties
) :
    Base::Base(properties)
{
    m_MetaKey.Type = VD_DATA_MODEL_TYPE_META;
}

// ============================================================================================== //

namespace Graph {

// ============================================================================================== //

Node::Node() : 
    Data::Model::Base()
{
    // EMPTY!
}

Node::Node(
    const Meta& meta
) :     
    Data::Model::Base(meta)
{
    // EMPTY!
}

Node::Node(
    const Node& other
) :     
    Data::Model::Base(other)
{
    // EMPTY!
}

Node::Node(
    const Property::ListType& properties
) :     
    Data::Model::Base(properties)
{
    // EMPTY!
}

// ---------------------------------------------------------------------------------------------- //

Edge::Edge() : 
    Data::Model::Base()
{
    // EMPTY!
}

Edge::Edge(
    const Meta& meta
) :     
    Data::Model::Base(meta)
{
    // EMPTY!
}

Edge::Edge(
    const Edge& other
) :     
    Data::Model::Base(other),
    m_Src(other.m_Src), 
    m_Dst(other.m_Dst) 
{
    // EMPTY!
}

Edge::Edge(
    NodeKey src, NodeKey dst
) :     
    Data::Model::Base(),
    m_Src(src), 
    m_Dst(dst) 
{
    // EMPTY!
}

Edge::Edge(
    NodeKey src, NodeKey dst,
    const Property::ListType& properties
) :     
    Data::Model::Base(properties),
    m_Src(src), 
    m_Dst(dst) 
{
    // EMPTY!
}

NodeKey
Edge::GetSrcNodeKey() const
{
    return m_Src;
}

NodeKey
Edge::GetDstNodeKey() const
{
    return m_Dst;
}

// ============================================================================================== //

Connected::Connected()  :   
    Base::Base(),
    m_LastNodeSlot(0),
    m_LastEdgeSlot(0)
{
    m_MetaKey.Type = VD_DATA_MODEL_TYPE_GRAPH;
}

Connected::Connected(
    const Meta& meta
) :
    Base::Base(meta),
    m_LastNodeSlot(0),
    m_LastEdgeSlot(0)
{
    m_MetaKey.Type = VD_DATA_MODEL_TYPE_GRAPH;
}

Connected::Connected(
    const Property::ListType& properties
) :
    Data::Model::Base(properties),
    m_LastNodeSlot(0),
    m_LastEdgeSlot(0)
{
    m_MetaKey.Type = VD_DATA_MODEL_TYPE_GRAPH;
}

size_t
Connected::GetEdgeList(
    EdgeListType& result) const
{
    result.clear();
    EdgeMapType::const_iterator it;
    for(it = m_Edges.begin(); it != m_Edges.end(); it++)
        result.push_back(it->second);
    return result.size();
}

size_t
Connected::GetNodeList(
    NodeListType& result) const
{
    result.clear();
    NodeMapType::const_iterator it;
    for(it = m_Nodes.begin(); it != m_Nodes.end(); it++)
        result.push_back(it->second);
    return result.size();
}

bool
Connected::GetNodeMeta(
    NodeKey key, Meta& meta) const
{
    if(MetaKey::IsValid(key) == false)
        return false;

    if(key.Type == VD_DATA_MODEL_TYPE_NODE)
    {
        NodeMapType::const_iterator it;
        it = m_Nodes.find(key.Id);
        if(it != m_Nodes.end())
        {
            meta = it->second;
            return true;
        }
    }
    return false;
}

bool
Connected::GetEdgeMeta(
    EdgeKey key, Meta& meta) const
{
    if(MetaKey::IsValid(key) == false)
        return false;

    if(key.Type == VD_DATA_MODEL_TYPE_EDGE)
    {
        EdgeMapType::const_iterator it;
        it = m_Edges.find(key.Id);
        if(it != m_Edges.end())
        {
            meta = it->second;
            return true;
        }
    }
    return false;
}

bool
Connected::GetMeta(
    MetaKey key, Meta& meta) const
{
    if(MetaKey::IsValid(key) == false)
        return false;

    if(key.Type == VD_DATA_MODEL_TYPE_NODE)
    {
        return GetNodeMeta(key, meta);
    }
    else if(key.Type == VD_DATA_MODEL_TYPE_EDGE)
    {
        return GetEdgeMeta(key, meta);
    }
    return false;
}

NodeKey 
Connected::AddNode()
{
    Node node;
    node.m_MetaKey.Id = m_LastNodeSlot++;
    node.m_MetaKey.Type = VD_DATA_MODEL_TYPE_NODE;
    m_Nodes[node.m_MetaKey.Id] = node;
    return node.m_MetaKey;
}

NodeKey 
Connected::AddNode(
    const Meta& meta)
{
    Property::ListType properties;
    meta.GetPropertyList(properties);
    return AddNode(properties);
}

NodeKey 
Connected::AddNode(
    const Property::ListType& properties)
{
    Node node(properties);
    node.m_MetaKey.Id = m_LastNodeSlot++;
    node.m_MetaKey.Type = VD_DATA_MODEL_TYPE_NODE;
    m_Nodes[node.m_MetaKey.Id] = node;
    return node.m_MetaKey;
}

NodeKey 
Connected::SetNode(
    NodeKey key, const Meta& meta)
{
    Property::ListType properties;
    meta.GetPropertyList(properties);
    Node node(properties);
    node.m_MetaKey.Id = key.Id;
    node.m_MetaKey.Type = VD_DATA_MODEL_TYPE_NODE;
    m_Nodes[node.m_MetaKey.Id] = node;
    return node.m_MetaKey;
}

bool Connected::RemoveNode(
    NodeKey key)
{
    if(MetaKey::IsValid(key) == false)
        return false;

    if(key.Type == VD_DATA_MODEL_TYPE_NODE)
    {
        NodeMapType::const_iterator it;
        it = m_Nodes.find(key.Id);
        if(it != m_Nodes.end())
        {
            m_Nodes.erase(it);
            return true;
        }
    }

    return false;
}

EdgeKey 
Connected::AddEdge(
    NodeKey src, NodeKey dst)
{
    Edge edge(src, dst);
    edge.m_MetaKey.Id = m_LastEdgeSlot++;
    edge.m_MetaKey.Type = VD_DATA_MODEL_TYPE_EDGE;
    m_Edges[edge.m_MetaKey.Id] = edge;
    return edge.m_MetaKey;
}

EdgeKey 
Connected::AddEdge(
    NodeKey src, NodeKey dst, const Meta& meta)
{
    Property::ListType properties;
    meta.GetPropertyList(properties);
    return AddEdge(src, dst, properties);
}

EdgeKey 
Connected::AddEdge(
    NodeKey src, NodeKey dst, 
    const Property::ListType& properties)
{
    Edge edge(src, dst, properties);
    edge.m_MetaKey.Id = m_LastEdgeSlot++;
    edge.m_MetaKey.Type = VD_DATA_MODEL_TYPE_EDGE;
    m_Edges[edge.m_MetaKey.Id] = edge;
    return edge.m_MetaKey;
}

EdgeKey 
Connected::SetEdge(
    EdgeKey key, NodeKey src, NodeKey dst, const Meta& meta)
{
    RemoveEdge(key);
    Property::ListType properties;
    meta.GetPropertyList(properties);
    Edge edge(src, dst, properties);
    edge.m_MetaKey.Id = key.Id;
    edge.m_MetaKey.Type = VD_DATA_MODEL_TYPE_EDGE;
    m_Edges[edge.m_MetaKey.Id] = edge;
    return edge.m_MetaKey;
}


bool Connected::RemoveEdge(
    EdgeKey key)
{
    if(MetaKey::IsValid(key) == false)
        return false;

    if(key.Type == VD_DATA_MODEL_TYPE_EDGE)
    {
        EdgeMapType::const_iterator it;
        it = m_Edges.find(key.Id);
        if(it != m_Edges.end())
        {
            m_Edges.erase(it);
            return true;
        }
    }

    return false;
}

bool Connected::Remove(
    MetaKey key)
{
    if(MetaKey::IsValid(key) == false)
        return false;

    if(key.Type == VD_DATA_MODEL_TYPE_NODE)
    {
        return RemoveNode(key);
    }
    else if(key.Type == VD_DATA_MODEL_TYPE_EDGE)
    {
        return RemoveEdge(key);
    }

    return false;
}

void Connected::Clear()
{
    m_Edges.clear();
    m_Nodes.clear();

    m_LastEdgeSlot = 0;
    m_LastNodeSlot = 0;
}
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

    bool operator()(const Property::ValueType& v) const
    {
        static const std::string open_child("[ ");
        static const std::string close_child("] ");

        Result << open_child;
        boost::apply_visitor( ToJsonVisitor<Archive>(Result), v );
        Result << close_child;

        return true;
    }

    bool operator()(const std::vector<Property::ValueType>& v) const
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
    const Property& property)
{
    static const std::string define_prop(" : ");

    output << "\"" << property.GetName() << "\"" << define_prop;
    boost::apply_visitor( ToJsonVisitor<std::ostream>(output), property.GetValue() );
    return true;
}

bool ToJson(
    std::ostream& output,
    const Node& node)
{
    static const std::string open_props("{ ");
    static const std::string close_props(" }");
    static const std::string define_prop(" : ");
    static const std::string next_list(", ");
    static const std::string end_list(" ");
    static const std::string sep(" ");

    MetaKey key = node.GetMetaKey();

    output << open_props;
    output << "\"Id\"" << define_prop;
    output << key.Id << next_list;

    Property::ListType properties;
    node.GetPropertyList(properties);

    size_t n = 0;
    size_t size = properties.size();
    Property::ListType::const_iterator it;
    for(it = properties.begin(); it != properties.end(); it++)
    {
        bool more = (n < size - 1);
        Serialize::ToJson(output, (*it));
        output << (more ? next_list : end_list);
        n++;
    }
    output << close_props;
    return true;
}

bool ToJson(
    std::ostream& output,
    const Edge& edge)
{
    static const std::string open_props("{ ");
    static const std::string close_props(" }");
    static const std::string define_prop(" : ");
    static const std::string next_list(", ");
    static const std::string end_list(" ");
    static const std::string sep(" ");

    MetaKey key = edge.GetMetaKey();
    NodeKey src = edge.GetSrcNodeKey();
    NodeKey dst = edge.GetDstNodeKey();

    output << open_props;
    output << "\"Id\""  << define_prop << key.Id << next_list;
    output << "\"Src\"" << define_prop << src.Id << next_list;
    output << "\"Dst\"" << define_prop << dst.Id << next_list;

    Property::ListType properties;
    edge.GetPropertyList(properties);

    size_t n = 0;
    size_t size = properties.size();
    Property::ListType::const_iterator it;
    for(it = properties.begin(); it != properties.end(); it++)
    {
        bool more = (n < size - 1);
        Serialize::ToJson(output, (*it));
        output << (more ? next_list : end_list);
        n++;
    }
    output << close_props;
    return true;
}

bool ToJson(
    std::ostream& output,
    const std::vector<Node>& nodes)
{
    static const std::string next_list(", ");
    static const std::string end_list(" ");

    size_t size = nodes.size();
    for(size_t n = 0; n < size; n++)
    {
        bool more = (n < size - 1);
        Serialize::ToJson(output, nodes[n]);
        output << (more ? next_list : end_list);
        output << std::endl;
    }
    return true;
}

bool ToJson(
    std::ostream& output,
    const std::vector<Edge>& edges)
{
    static const std::string next_list(", ");
    static const std::string end_list(" ");

    size_t size = edges.size();
    for(size_t n = 0; n < size; n++)
    {
        bool more = (n < size - 1);
        Serialize::ToJson(output, edges[n]);
        output << (more ? next_list : end_list);
        output << std::endl;
    }
    return true;
}

// ============================================================================================== //

} // end namespace: Serialize
} // end namespace: Graph
} // end namespace: Model
} // end namespace: Data

// ============================================================================================== //
