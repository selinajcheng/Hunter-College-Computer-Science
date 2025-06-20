#include "Graph.hpp"
#include <fstream>

namespace VertexCover {
    // Departure (HH: MM) - CODE | Destination (HH: MM) - CODE
    Graph readFromFile(const std::string& filename) {
        // open the file for reading
        std::ifstream fin(filename);
        if (fin.fail()) {
            throw std::runtime_error("Failed to open file: " + filename);
        }

        Graph g;
        std::string line;

        // read in the file line-by-line
        while (std::getline(fin, line)) {
            // departure code is always from index 21-24
            // destination code is always from index 51-54
            std::string departure = line.substr(21, 3);
            std::string destination = line.substr(51, 3);
            // std::cout << departure << " " << destination << std::endl;

            // insert edge twice in the undirected graph
            g[departure].insert(destination);
            g[destination].insert(departure);
        }

        return g;
    }

    std::unordered_set<Vertex> cover_graph(Graph g) {
        std::unordered_set<Vertex> cover;

        // continue until all edges are covered
        while (!g.empty()) {
            Vertex current_vertex;
            size_t max_degree = 0;
            // find vertex with max degree
            for (const auto& [vertex, neighbors] : g) {
                if (neighbors.size() > max_degree) {
                    max_degree = neighbors.size();
                    current_vertex = vertex;
                }
            }

            // add to cover set
            cover.insert(current_vertex);

            for (const auto& neighbor : g[current_vertex]) {
                // remove all edges from current vertex
                g[neighbor].erase(current_vertex);
                // remove current vertex if no edges connect it to the graph
                if (g[neighbor].empty()) {
                    g.erase(neighbor);
                }
            }

            g.erase(current_vertex);
        }
        return cover;
    }
}