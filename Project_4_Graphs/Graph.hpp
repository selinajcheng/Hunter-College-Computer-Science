#pragma once

#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <algorithm>
#include <stdexcept>

using Vertex = std::string;
using Neighbors = std::unordered_set<Vertex>;
using Graph = std::unordered_map<Vertex, Neighbors>;


namespace VertexCover {
/**
 * @brief Generates a sub-optimal minimumum vertex cover
 * by repeatedly choosing the largest degree vertex & 
 * adding it to the cover set.
 *
 * @param g (Graph) The graph object 
 * for which to generate a vertex cover. 
 * NOTE: This is NOT a const. reference
 *
 * @return (std::unordered_set<Vertex>) The set of vertices 
 * that forms a vertex cover of the graph.
 */
std::unordered_set<Vertex> cover_graph(Graph g);
}