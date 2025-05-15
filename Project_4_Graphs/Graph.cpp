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
            // departure code is always from index 23-26
            // destination code is always from index 53-56
            std::string departure = line.substr(23, 3);
            std::string destination = line.substr(53, 3);

            // insert edge twice in the undirected graph
            g[departure].insert(destination);
            g[destination].insert(departure);
        }

        return g;
    }

    std::unordered_set<Vertex> cover_graph(Graph g) {
        // stub
        return std::unordered_set<Vertex>();
    }
}