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

#include <string>
#include <cstdlib>
#include "neo4j.h"

// ============================================================================================== //

namespace Neo4j  {
namespace Client {
namespace Rest {
    
// ============================================================================================== //

const std::string Basic::AcceptType("application/json");
const std::string Basic::ContentType("application/json");

// ----------------------------------------------------------------------------------------------- //

Basic::Basic(
    boost::asio::io_service& service
) 
    : m_Client(service) 
{
    // EMPTY!
}

Basic::~Basic()
{
    Disconnect();
}

bool 
Basic::Connect(
    const std::string& host, 
    const std::string& port, 
    const std::string& settings)
{
    try
    {
        m_Client.Get(host, port, "db/data/node/0", AcceptType);
    }
    catch (std::exception& e)
    {
        std::cout << "Exception: " << e.what() << "\n";
    }

    m_Host = host;
    m_Port = port;
    m_Settings = settings;
    return true;
}

bool 
Basic::Disconnect()
{
    m_Host = std::string();
    m_Port = std::string();
    m_Settings = std::string();
    return true;
}

bool 
Basic::Import(
    const NodeList& nodes, const EdgeList& edges)
{
    return false;
}

bool 
Basic::AddNodes(const NodeList& nodes)
{
    return false;
}

bool 
Basic::AddEdges(const EdgeList& edges)
{
    return false;
}

bool 
Basic::Serialize(
    std::ostream& output, const NodeList& nodes) const
{
    return Data::Model::Graph::Serialize::ToJson(output, nodes);
}

bool 
Basic::Serialize(
    std::ostream& output, const EdgeList& edges) const
{
    return Data::Model::Graph::Serialize::ToJson(output, edges);
}

// ============================================================================================== //

}  // end namespace: Rest
}  // end namespace: Client
}  // end namespace: Neo4J

// ============================================================================================== //
