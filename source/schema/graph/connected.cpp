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

#include "schema/graph/connected.h"
#include "schema/data/model.h"

#include <boost/variant.hpp>
#include <boost/serialization/variant.hpp>

// ============================================================================================== //

VD_SCHEMA_NAMESPACE_BEGIN();

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
    m_MetaKey.Type = Data::VD_DATA_MODEL_TYPE_GRAPH;
}

Connected::Connected(
    const Meta& meta
) :
    Base::Base(meta),
    m_LastNodeSlot(0),
    m_LastEdgeSlot(0)
{
    m_MetaKey.Type = Data::VD_DATA_MODEL_TYPE_GRAPH;
}

Connected::Connected(
    const Property::ListType& properties
) :
    Data::Model::Base(properties),
    m_LastNodeSlot(0),
    m_LastEdgeSlot(0)
{
    m_MetaKey.Type = Data::VD_DATA_MODEL_TYPE_GRAPH;
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

    if(key.Type == Data::VD_DATA_MODEL_TYPE_NODE)
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

    if(key.Type == Data::VD_DATA_MODEL_TYPE_EDGE)
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

    if(key.Type == Data::VD_DATA_MODEL_TYPE_NODE)
    {
        return GetNodeMeta(key, meta);
    }
    else if(key.Type == Data::VD_DATA_MODEL_TYPE_EDGE)
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
    node.m_MetaKey.Type = Data::VD_DATA_MODEL_TYPE_NODE;
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
    node.m_MetaKey.Type = Data::VD_DATA_MODEL_TYPE_NODE;
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
    node.m_MetaKey.Type = Data::VD_DATA_MODEL_TYPE_NODE;
    m_Nodes[node.m_MetaKey.Id] = node;
    return node.m_MetaKey;
}

bool Connected::RemoveNode(
    NodeKey key)
{
    if(MetaKey::IsValid(key) == false)
        return false;

    if(key.Type == Data::VD_DATA_MODEL_TYPE_NODE)
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
    edge.m_MetaKey.Type = Data::VD_DATA_MODEL_TYPE_EDGE;
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
    edge.m_MetaKey.Type = Data::VD_DATA_MODEL_TYPE_EDGE;
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
    edge.m_MetaKey.Type = Data::VD_DATA_MODEL_TYPE_EDGE;
    m_Edges[edge.m_MetaKey.Id] = edge;
    return edge.m_MetaKey;
}


bool Connected::RemoveEdge(
    EdgeKey key)
{
    if(MetaKey::IsValid(key) == false)
        return false;

    if(key.Type == Data::VD_DATA_MODEL_TYPE_EDGE)
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

    if(key.Type == Data::VD_DATA_MODEL_TYPE_NODE)
    {
        return RemoveNode(key);
    }
    else if(key.Type == Data::VD_DATA_MODEL_TYPE_EDGE)
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

} // end namespace: Graph

// ============================================================================================== //

VD_SCHEMA_NAMESPACE_END();

// ============================================================================================== //
