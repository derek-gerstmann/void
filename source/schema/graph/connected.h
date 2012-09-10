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

#ifndef VD_SCHEMA_GRAPH_CONNECTED_INCLUDED
#define VD_SCHEMA_GRAPH_CONNECTED_INCLUDED

// ============================================================================================== //

#include "schema/schema.h"
#include "schema/graph.h"

// ============================================================================================== //

VD_SCHEMA_NAMESPACE_BEGIN();

// ============================================================================================== //

namespace Graph {

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

} // end namespace: Graph

// ============================================================================================== //

VD_SCHEMA_NAMESPACE_END();

// ============================================================================================== //

#endif  // VD_DATA_MODEL_PROPERTY_GRAPH_INCLUDED

