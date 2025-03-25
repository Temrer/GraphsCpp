#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>

void generate_random_graph_to_file(uint32_t n, uint32_t m,
                                   const std::string& filename) {
    // Initialize random seed
    std::srand(static_cast<unsigned int>(std::time(0)));

    // Open file for writing
    std::ofstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open file for writing!" << std::endl;
        return;
    }

    // Write the number of vertices (n) and edges (m) to the file
    file << n << " " << m << std::endl;

    // Set to track which vertices have been added as either parent or child
    std::unordered_set<uint32_t> vertices_in_graph;

    // Generate and write m random edges to the file
    for (uint32_t i = 0; i < m; ++i) {
        uint32_t origin = std::rand() % n;  // Random origin vertex
        uint32_t target = std::rand() % n;  // Random target vertex

        // Ensure origin and target are not the same (no self-loops)
        while (origin == target) {
            target = std::rand() % n;
        }

        // Check if both vertices already exist (have been added to the graph)
        // Add the origin and target vertices to the set
        vertices_in_graph.insert(origin);
        vertices_in_graph.insert(target);

        // Randomly assign a cost to the edge
        uint32_t cost = std::rand() % 100 + 1;  // Random cost between 1 and 100

        // Write edge to file: origin, target, and cost
        file << origin << " " << target << " " << cost << std::endl;
    }

    // Close the file
    file.close();

    // Output info about the generated graph
    std::cout << "Random graph saved to " << filename << std::endl;
    std::cout << "Vertices included in the graph: ";

    // Print the vertices that were included in the graph
    for (const auto& vertex : vertices_in_graph) {
        std::cout << vertex << " ";
    }
    std::cout << std::endl;
}

int main() {
    uint32_t n, m;
    std::string filename;
    std::cout << "Input name of file: ";
    std::cin >> filename;

    std::cout << "Input number of vertices and edges\n";
    std::cin >> n >> m;

    generate_random_graph_to_file(n, m, filename);
}
