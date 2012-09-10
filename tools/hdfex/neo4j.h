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

#ifndef VD_DB_NEO4J_CLIENT_INCLUDED
#define VD_DB_NEO4J_CLIENT_INCLUDED

// ============================================================================================== //

#include <string>
#include <cstdlib>
#include "rest.h"
#include "dmpg.h"

// ============================================================================================== //

namespace Neo4j  {
namespace Client {
namespace Rest {
    
// ============================================================================================== //

class Basic
{
public:
    static const std::string                                AcceptType;
    static const std::string                                ContentType;

public:
    typedef Data::Model::Graph::Node                        NodeType;
    typedef Data::Model::Graph::Edge                        EdgeType;
    typedef Data::Model::Property                           PropertyType;

    typedef std::vector< PropertyType::ValueType >          PropertyValueList;
    typedef std::vector< PropertyType >                     PropertyList;
    typedef std::vector< EdgeType >                         EdgeList;
    typedef std::vector< NodeType >                         NodeList;

public:

    Basic(boost::asio::io_service& service);
    ~Basic();

    bool Connect(
        const std::string& host, 
        const std::string& port, 
        const std::string& settings);

    bool Disconnect();

    bool Import(
        const NodeList& nodes, const EdgeList& edges);

    bool AddNodes(const NodeList& nodes);
    bool AddEdges(const EdgeList& edges);

    bool Serialize(std::ostream& output, const NodeList& nodes) const;
    bool Serialize(std::ostream& output, const EdgeList& edges) const;

private:

    std::string             m_Host;
    std::string             m_Port;
    std::string             m_Settings;
    ::Rest::Client::Async   m_Client;
};

// ============================================================================================== //

}  // end namespace: Rest
}  // end namespace: Client
}  // end namespace: Neo4J

// ============================================================================================== //

#endif  // VD_DB_NEO4J_CLIENT_REST_INCLUDED
