#include <chrono>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <execution>
#include <fstream>
#include <functional>
#include <iostream>
#include <ostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

struct Edge {
    uint32_t parent;
    uint32_t child;
    uint32_t cost;
};

struct pair_hash {
    template <class T1, class T2>
    std::size_t operator()(const std::pair<T1, T2> &p) const {
        auto h1 = std::hash<T1>{}(p.first);
        auto h2 = std::hash<T2>{}(p.second);
        return h1 ^ (h2 << 1);
    }
};

void read_data(std::unordered_map<uint32_t, std::vector<uint32_t>> &inbound,
               std::unordered_map<uint32_t, std::vector<uint32_t>> &outbound,
               std::unordered_map<std::pair<uint32_t, uint32_t>, uint32_t,
                                  pair_hash> &costs,
               uint32_t &Vertices, uint32_t &Edges, char *filename) {
    std::ifstream input(filename);
    if (!input.is_open()) {
        std::cerr << "Failed to open file " << filename << std::endl;
        return;
    }
    std::cout << "File opened\n";

    uint32_t vertices = 0, edges = 0;
    input >> vertices >> edges;
    if (vertices == 0) {
        std::cerr << "The file is empty\n";
        return;
    }

    for (uint32_t i = 0; i < vertices; i++) {
        inbound[i] = {};
        outbound[i] = {};
    }

    uint32_t parent, child, cost;
    while (edges--) {
        if (!(input >> parent >> child >> cost)) {
            std::cerr << "Error reading edge data\n";
            break;
        }
        outbound[parent].push_back(child);
        inbound[child].push_back(parent);
        costs[{parent, child}] = cost;
    }

    Vertices = vertices;
    Edges = edges;
    input.close();
}

int main(int argc, char **argv) {
    auto start_time = std::chrono::high_resolution_clock::now();
    if (argc < 2) {
        std::cout << "No file specified. Exiting...\n";
        return 0;
    }

    std::unordered_map<uint32_t, std::vector<uint32_t>> inbound;
    std::unordered_map<uint32_t, std::vector<uint32_t>> outbound;
    std::unordered_map<std::pair<uint32_t, uint32_t>, uint32_t, pair_hash>
        costs;

    uint32_t vertices, edges;
    read_data(inbound, outbound, costs, vertices, edges, argv[1]);

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
        end_time - start_time);

    std::cout << "Process took " << duration.count()
              << " milliseconds to perform.\n";
    return 0;
}

