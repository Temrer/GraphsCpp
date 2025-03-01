#include <chrono>
#include <cstdint>
#include <cstdio>
#include <execution>
#include <fstream>
#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

struct Edge {
    uint32_t parent;
    uint32_t child;
    uint32_t cost;
};
// assuming that there won't be a whole lot of edges to a single vertex, we can
// use linked lists to store inbound and outbounding edges
// Because the application is not supposed to traverse the data we can simply us
// memory-efficient linked lists

struct LinkedList {
    uint32_t value;
    LinkedList *next;

    explicit LinkedList(uint32_t v) : value(v), next(nullptr) {}
};

struct pair_hash {
    template <class T1, class T2>
    std::size_t operator()(const std::pair<T1, T2> &p) const {
        auto h1 = std::hash<T1>{}(p.first);
        auto h2 = std::hash<T2>{}(p.second);
        return h1 ^ (h2 << 1);  // Combine hashes
    }
};

// A function that will read the specified file and load it into the program
void read_data(std::unordered_map<uint32_t, uint32_t *> &inbound,
               std::unordered_map<uint32_t, uint32_t *> &outbound,
               std::unordered_map<std::pair<uint32_t, uint32_t>, uint32_t,
                                  pair_hash> &costs,
               uint32_t &Vertices, uint32_t &Edges, char *filename,
               uint32_t vertex_buffer) {
    std::ifstream input(filename);
    if (!input.is_open()) {
        // Raise an error telling that the file was not opened succesfully
        std::cerr << "Failed to open file " << filename << std::endl;
        return;
    }
    std::cout << "File opened\n";

    uint32_t vertices = 0, edges;
    input >> vertices >> edges;
    if (vertices == 0) {
        // Raise an error telling that the file is empty
        std::cerr << "The file is empty\n";
        return;
    }

    uint16_t in_size[vertices] = {0};
    uint16_t out_size[vertices] = {0};
    // I don't expect very large out degrees so
    // a 16 bit representation should suffice

    std::vector<LinkedList *> outbound_Ll(vertices, nullptr);
    std::vector<LinkedList *> inbound_Ll(vertices, nullptr);
    // A list where the pointers to the linked lists will be stored

    uint32_t parent, child, cost;  // Read all data and store it
    std::cout << "Reading from file...\n";
    LinkedList *outboundNode;
    LinkedList *inboundNode;
    for (uint32_t i = 0; i < edges; i++) {
        input >> parent >> child >> cost;
        // increase the out and in size
        out_size[parent]++;
        in_size[child]++;

        // Add cost to the cost map
        costs[{parent, child}] = cost;

        // Add to outbound adjacency list (linked list per vertex)
        outboundNode = new LinkedList(child);
        outboundNode->next = outbound_Ll[parent];  // Insert at front of list
        outbound_Ll[parent] = outboundNode;

        // Add to inbound adjacency list (linked list per vertex)
        inboundNode = new LinkedList(parent);
        inboundNode->next = inbound_Ll[child];  // Insert at front of list
        inbound_Ll[child] = inboundNode;
    }

    std::cout << parent << " " << child << " " << cost << "\n";

    // after reading the data, store it in a in-out fast, memory-efficient way
    // create arrays for outbound edges
    // we will store in the first cell the size of the array
    for (uint32_t i = 0; i < vertices; i++) {
        uint32_t *out = reinterpret_cast<uint32_t *>(
            malloc(sizeof(uint32_t) * (out_size[i] + 1 + vertex_buffer)));

        LinkedList *node, *next;
        node = outbound_Ll[i];
        out[0] = out_size[i];
        // check if the list is not empty <good practice>
        if (node != nullptr) {
            for (int j = 0; j < out_size[i]; j++) {
                out[j + 1] =
                    node->value;    // Store the value of the current node
                next = node->next;  // Get the next node
                delete node;        // Free the memory of the current node
                node = next;        // Move to the next node in the list
            }
        }
        outbound[i] = out;  // passing the pointer of the array to the map
    }

    // create arrays for inbound edges
    for (uint32_t i = 0; i < vertices; i++) {
        uint32_t *in = reinterpret_cast<uint32_t *>(
            malloc(sizeof(uint32_t) * (in_size[i] + 1 + vertex_buffer)));

        LinkedList *node, *next;
        node = inbound_Ll[i];
        in[0] = in_size[i];
        // check if the list is not empty <good practice>
        if (node != nullptr) {
            for (int j = 0; j < in_size[i]; j++) {
                in[j + 1] = node->value;  // Store the value of the current node
                next = node->next;        // Get the next node
                delete node;              // Free the memory of the current node
                node = next;              // Move to the next node in the list
            }
        }
        inbound[i] = in;  // passing the pointer of the array to the map
    }

    Vertices = vertices;
    Edges = edges;
    input.close();
}

void create_order() {


};

// TODO(Temeraire): free allocated memory used when initializing vectors
int main(int argc, char **argv) {
    std::cout << argc << " | " << argv;
    auto start_time = std::chrono::high_resolution_clock::now();
    if (argc < 2) {
        std::cout << "No file specified. Exiting...\n";
        return 0;
    }

    uint32_t vertex_buffer = 10;
    // reading overhead-buffer size for the lists of vertices
    if (argc >= 3) {
        vertex_buffer = std::stoi(argv[2]);
    }

    std::unordered_map<uint32_t, uint32_t *> inbound;
    std::unordered_map<uint32_t, uint32_t *> outbound;
    std::unordered_map<std::pair<uint32_t, uint32_t>, uint32_t, pair_hash>
        costs;

    uint32_t vertices, edges;
    read_data(inbound, outbound, costs, vertices, edges, argv[1],
              vertex_buffer);

    // Free dynamically allocated memory before exiting
    for (auto &entry : outbound) {
        free(entry.second);  // Free each allocated array
    }
    for (auto &entry : inbound) {
        free(entry.second);
    }
    std::cout.flush();

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
        end_time - start_time);

    std::cout << "Process took " << duration.count()
              << " milliseconds to perform.\n";
    return 0;
}
