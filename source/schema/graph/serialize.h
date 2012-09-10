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

#ifndef VD_SCHEMA_GRAPH_SERIALIZE_INCLUDED
#define VD_SCHEMA_GRAPH_SERIALIZE_INCLUDED

// ============================================================================================== //

#include "schema/schema.h"
#include "schema/graph/graph.h"
#include "schema/graph/connected.h"

// ============================================================================================== //

VD_SCHEMA_NAMESPACE_BEGIN();

// ============================================================================================== //

namespace Graph {
namespace Serialize {

// ============================================================================================== //

bool ToJson(
    std::ostream& output, const Graph::Property& property);

bool ToJson(
    std::ostream& output, const Graph::Node& node);

bool ToJson(
    std::ostream& output, const Graph::Edge& edge);

bool ToJson(
    std::ostream& output, const std::vector<Graph::Node>& nodes);

bool ToJson(
    std::ostream& output, const std::vector<Graph::Edge>& edges);

// ============================================================================================== //

} // end namespace: Serialize
} // end namespace: Graph

// ============================================================================================== //

VD_SCHEMA_NAMESPACE_END();

// ============================================================================================== //

#endif  // VD_SCHEMA_GRAPH_SERIALIZE_INCLUDED

