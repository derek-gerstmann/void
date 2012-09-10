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

#ifndef VD_DATA_MODEL_PROPERTY_GRAPH_INCLUDED
#define VD_DATA_MODEL_PROPERTY_GRAPH_INCLUDED

// ============================================================================================== //

#include "std.h"

#include <boost/variant.hpp>
#include <boost/unordered_map.hpp>
#include <boost/serialization/variant.hpp>

// ============================================================================================== //

namespace Data {

// ============================================================================================== //

struct Constants
{
    static const uint64_t InvalidMetaKeyId;
};

// ============================================================================================== //

enum ModelTypeId
{
    VD_DATA_MODEL_TYPE_INVALID,
    VD_DATA_MODEL_TYPE_META,
    VD_DATA_MODEL_TYPE_VALUE,
    VD_DATA_MODEL_TYPE_PROPERTY,
    VD_DATA_MODEL_TYPE_EDGE,
    VD_DATA_MODEL_TYPE_NODE,
    VD_DATA_MODEL_TYPE_GRAPH,
    VD_DATA_MODEL_TYPE_LAST_ID
};

struct MetaKey
{
    uint64_t    Id;
    ModelTypeId Type;

    MetaKey();
    static bool IsValid(MetaKey mk);
};

typedef std::vector<MetaKey> MetaKeyList;

// ============================================================================================== //

namespace Model {

// ============================================================================================== //

class Property
{
public:
    typedef boost::make_recursive_variant<bool,
        int8_t,  uint8_t,  int16_t, uint16_t, 
        int32_t, uint32_t, int64_t, uint64_t,
        float, double, std::string, 
        std::vector< boost::recursive_variant_ > >::type    ValueType;
    typedef boost::unordered_map<std::string, Property>     MapType;
    typedef std::vector<Property>                           ListType;
    typedef std::vector<std::string>                        NameListType;
    typedef std::vector<ValueType>                          ValueListType;
    static const ValueType                                  InvalidValue;

public:

    Property();
    Property(const Property& other);
    Property(const std::string& n, const ValueType& v); 

    const std::string& GetName() const;
    const ValueType& GetValue() const;

protected:
    std::string   m_Name;
    ValueType     m_Value;
};

class Base 
{
public:
    Base();
    Base(const Base& other);
    Base(const Property::ListType& properties);

    void 
    Set(const std::string& name, const Property::ValueType& value);

    const Property::ValueType& 
    Get(const std::string& name, const Property::ValueType& missing = Property::InvalidValue) const;
    
    bool 
    Remove(const std::string& name);

    bool GetPropertyList(Property::ListType& result) const;
    bool GetPropertyNames(Property::NameListType& result) const;

    MetaKey GetMetaKey() const;

protected:
    MetaKey                     m_MetaKey;
    Property::MapType           m_Properties;
};

class Meta : public Base
{
public:
    Meta();
    Meta(const Base& other);
    Meta(const Property::ListType& properties);
};

// ============================================================================================== //

namespace Graph {

// ============================================================================================== //

typedef MetaKey NodeKey;
typedef MetaKey EdgeKey;

// ============================================================================================== //

class Connected;

// ============================================================================================== //

class Node : public Data::Model::Base
{
    friend class Graph::Connected;

public:
    Node();
    Node(const Meta& meta);
    Node(const Node& other);
    Node(const Property::ListType& properties);
};

// ============================================================================================== //

class Edge : public Data::Model::Base
{
    friend class Graph::Connected;

public:
    Edge();
    Edge(const Meta& meta);
    Edge(const Edge& other);
    Edge(NodeKey src, NodeKey dst);
    Edge(NodeKey src, NodeKey dst, const Property::ListType& properties);

    NodeKey GetSrcNodeKey() const;
    NodeKey GetDstNodeKey() const;

protected:

    NodeKey m_Src;
    NodeKey m_Dst;
};

// ============================================================================================== //

class Connected : public Data::Model::Base
{
public:
    typedef boost::unordered_map<size_t, Node>     NodeMapType;
    typedef boost::unordered_map<size_t, Edge>     EdgeMapType;

    typedef std::vector<Node>                      NodeListType;
    typedef std::vector<Edge>                      EdgeListType;

public:
    Connected();
    Connected(const Meta& meta);
    Connected(const Property::ListType& properties);

    NodeKey AddNode();
    NodeKey AddNode(const Meta& meta);
    NodeKey AddNode(const Property::ListType& properties);
    bool GetNodeMeta(NodeKey key, Meta& meta) const;
    size_t GetNodeList(NodeListType& nodes) const;
    NodeKey SetNode(NodeKey key, const Meta& meta);
    bool RemoveNode(NodeKey item);
    
    EdgeKey AddEdge(NodeKey src, NodeKey dst);
    EdgeKey AddEdge(NodeKey src, NodeKey dst, const Meta& meta);
    EdgeKey AddEdge(NodeKey src, NodeKey dst, const Property::ListType& properties);
    bool GetEdgeMeta(EdgeKey key, Meta& meta) const;
    size_t GetEdgeList(EdgeListType& edges) const;
    EdgeKey SetEdge(EdgeKey key, NodeKey src, NodeKey dst, const Meta& meta);
    bool RemoveEdge(EdgeKey item);

    bool GetMeta(MetaKey key, Meta& meta) const;
    bool Remove(MetaKey item);
    void Clear();

protected:

    NodeMapType m_Nodes;
    EdgeMapType m_Edges;

    size_t m_LastNodeSlot;
    size_t m_LastEdgeSlot;
};

// ============================================================================================== //

namespace Serialize {

// ============================================================================================== //

bool ToJson(
    std::ostream& output, const Property& property);

bool ToJson(
    std::ostream& output, const Node& node);

bool ToJson(
    std::ostream& output, const Edge& edge);

bool ToJson(
    std::ostream& output, const std::vector<Node>& nodes);

bool ToJson(
    std::ostream& output, const std::vector<Edge>& edges);

// ============================================================================================== //

} // end namespace: Serialize

// ============================================================================================== //

} // end namespace: Graph
} // end namespace: Model
} // end namespace: Data

// ============================================================================================== //

#endif  // VD_DATA_MODEL_PROPERTY_GRAPH_INCLUDED

