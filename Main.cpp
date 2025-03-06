// Copyright (C) 2025 Temrer
// All rights reserved.
//
// This file is part of the project [Project Name].
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
// - Redistributions of source code must retain the above copyright notice,
//   this list of conditions, and the following disclaimer.
// - Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions, and the following disclaimer in the documentation
//   or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR "AS IS" AND ANY EXPRESS OR IMPLIED
// WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
// EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#include <sys/types.h>

#include <chrono>
#include <cstdint>
#include <cstdio>
<<<<<<< HEAD
#include <cstdlib>
#include <cstring>
#include <execution>
#include <fstream>
#include <functional>
=======
>>>>>>> new
#include <iostream>
#include <ostream>
#include <string>
#include <unordered_map>

#include "Data.h"
#include "Structures.h"

void print_menu() {
    const std::string Menu =
        "                MENU                 \n\
======================================\n\
1.  Show number of vertices           \n\
2.  Check edges between vertices      \n\
3.  Get in and out degrees of vertices\n\
4.  Get Out Map of vertices           \n\
5.  Get In Map of vertices            \n\
6.  Get weights of edges              \n\
7.  Modify weights of edges           \n\
8.  Add vertices                      \n\
9.  Remove vertices                   \n\
10. Add edges                         \n\
11. Remove edges                      \n\
12. Save a copy                       \n\
13. Open a Graph                      \n\
--------------------------------------\n";

<<<<<<< HEAD
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

    // initializing maps with an empty array
    for (uint32_t i = 0; i < vertices; i++) {
        uint32_t *empty_in = reinterpret_cast<uint32_t *>(malloc(32));
        uint32_t *empty_out = reinterpret_cast<uint32_t *>(malloc(32));
        empty_in[0] = 0;
        empty_out[0] = 0;
        inbound[i] = empty_in;
        outbound[i] = empty_out;
    }

    // Creating batches
    uint16_t MAX_BATCH_SIZE = 10000, batch_size = MAX_BATCH_SIZE;
    uint32_t consumed_edges = 0;

    uint32_t last_parent = 0, last_child = 0;
    while (consumed_edges < edges) {
        if (MAX_BATCH_SIZE > edges - consumed_edges)
            batch_size = edges - consumed_edges;

        std::unordered_map<uint32_t, uint16_t> index_map, reverse_index_map;

        uint16_t *in_size =
            reinterpret_cast<uint16_t *>(malloc(sizeof(uint16_t) * batch_size));
        uint16_t *out_size =
            reinterpret_cast<uint16_t *>(malloc(sizeof(uint16_t) * batch_size));

        for (uint16_t i = 0; i < batch_size; i++) {
            in_size[i] = 0;
            out_size[i] = 0;
        }
        // I don't expect very large out degrees so
        // a 16 bit representation should suffice

        // uint32_t arr_size = sizeof(LinkedList *) * batch_size;
        // LinkedList **outbound_Ll =
        //     reinterpret_cast<LinkedList **>(malloc(arr_size));
        // LinkedList **inbound_Ll =
        //     reinterpret_cast<LinkedList **>(malloc(arr_size));
        // if (outbound_Ll == nullptr || inbound_Ll == nullptr) {
        //     std::cerr << "Memory allocation failed!" << std::endl;
        //     return;
        // }

        LinkedList *outbound_Ll[batch_size] = {nullptr};
        LinkedList *inbound_Ll[batch_size] = {nullptr};

        // std::fill(outbound_Ll, outbound_Ll + arr_size, nullptr);
        // std::fill(inbound_Ll, inbound_Ll + arr_size, nullptr);

        // A list where the pointers to the linked lists will be stored
        uint32_t parent, child, cost, childi, parenti,
            local_index = 0;  // Read all data and store it
        std::cout << "Reading from file...\n";
        LinkedList *outboundNode;
        LinkedList *inboundNode;
        for (uint32_t i = 0; i < batch_size; i++) {
            input >> parent >> child >> cost;
            // find local index of parent and child
            parenti = index_map[parent];
            childi = index_map[child];
            if (index_map.find(parent) == index_map.end()) {
                parenti = local_index;
                reverse_index_map[local_index] = parent;
                index_map[parent] = local_index++;
            }
            if (index_map.find(child) == index_map.end()) {
                reverse_index_map[local_index] = child;
                childi = local_index;
                index_map[child] = local_index++;
            }

            // increase the out and in size
            out_size[parenti]++;
            in_size[childi]++;

            // Add cost to the cost map
            costs[{parent, child}] = cost;

            // Add to outbound adjacency list (linked list per vertex)
            outboundNode = new LinkedList(child);
            outboundNode->next =
                outbound_Ll[parenti];  // Insert at front of list
            outbound_Ll[parenti] = outboundNode;

            // Add to inbound adjacency list (linked list per vertex)
            inboundNode = new LinkedList(parent);
            inboundNode->next = inbound_Ll[childi];  // Insert at front of list
            inbound_Ll[childi] = inboundNode;
        }

        std::cout << parent << " " << child << " " << cost << "\n";

        // after reading the data, store it in a in-out fast, memory-efficient
        // way create arrays for outbound edges we will store in the first cell
        // the size of the array
        for (uint32_t i = 0; i < batch_size; i++) {
            // convert to usable data
            uint32_t i_value = reverse_index_map[i];

            // check previous array size
            uint32_t *vec = outbound[i_value];
            uint16_t prev_out = vec[0];
            uint16_t current_size = out_size[i] + prev_out;

            uint32_t *out = reinterpret_cast<uint32_t *>(
                malloc(sizeof(uint32_t) * (current_size + 1 + vertex_buffer)));
            LinkedList *node, *next;
            node = outbound_Ll[i];
            out[0] = current_size;

            // copy previous ellements to the new array
            for (uint16_t h = 1; h <= prev_out; h++) out[h] = vec[h];
            // free the previously allocated array
            free(vec);

            // check if the list is not empty <good practice>
            uint16_t osize = out_size[i];
            if (node != nullptr) {
                for (int j = 0; j < osize; j++) {
                    out[j + 1] =
                        node->value;    // Store the value of the current node
                    next = node->next;  // Get the next node
                    delete node;        // Free the memory of the current node
                    node = next;        // Move to the next node in the list
                }
            }
            outbound[i_value] =
                out;  // passing the pointer of the array to the map
        }

        // create arrays for inbound edges
        for (uint32_t i = 0; i < batch_size; i++) {
            // convert to usable data
            uint32_t i_value = reverse_index_map[i];

            // check previous array size
            uint32_t *vec = inbound[i_value];
            uint16_t prev_in = vec[0];
            uint16_t current_size = in_size[i] + prev_in;

            uint32_t *in = reinterpret_cast<uint32_t *>(
                malloc(sizeof(uint32_t) * (current_size + 1 + vertex_buffer)));
            LinkedList *node, *next;
            node = inbound_Ll[i];
            in[0] = current_size;

            // copy previous ellements to the new array
            for (uint16_t h = 1; h <= prev_in; h++) in[h] = vec[h];
            // free the previously allocated array
            free(vec);

            node = inbound_Ll[i];
            in[0] = in_size[i];
            // check if the list is not empty <good practice>
            if (node != nullptr) {
                for (int j = 0; j < in_size[i]; j++) {
                    in[j + 1] =
                        node->value;    // Store the value of the current node
                    next = node->next;  // Get the next node
                    delete node;        // Free the memory of the current node
                    node = next;        // Move to the next node in the list
                }
            }
            inbound[i_value] =
                in;  // passing the pointer of the array to the map
        }

        // free(outbound_Ll);
        // free(inbound_Ll);

        consumed_edges += batch_size;
    }
    Vertices = vertices;
    Edges = edges;
    input.close();
}

void create_order() {}
=======
    std::cout << Menu;
}

void choose_option(std::unordered_map<uint32_t, uint32_t *> &inbound,
                   std::unordered_map<uint32_t, uint32_t *> &outbound,
                   std::unordered_map<std::pair<uint32_t, uint32_t>, uint32_t,
                                      pair_hash> &costs,
                   uint32_t &Vertices, uint32_t &Edges) {
    int option = 0;
    std::string soption;
    while (!option) {
        std::cin >> soption;
        try {
            option = std::stoi(soption);
        } catch (std::exception &e) {
            std::cout << "Please choose a valid option\n";
        }
    }

    switch (option) {
        case 1:
            std::cout << Vertices << "\n";
            break;

        case 2: {
            uint16_t i = 0;
            Edge arg_edges[MAX_OPERATON_BUFFER];
            for (uint32_t k = 0; k <= MAX_OPERATON_BUFFER; k++)
                arg_edges[k] = NULL_EDGE;
            std::cout << "Input vertices between you want to check the "
                         "existence of an edge\n";
            std::string i1, i2;
            int bl = 1;
            while (bl) {
                std::cin >> i1;
                if (i1 == "confirm") {
                    bl = false;
                }
                if (bl) {
                    std::cin >> i2;
                    Edge e;
                    uint32_t a, b;
                    a = s2i(i1);
                    b = s2i(i2);
                    if (a == UINT32_MAX || b == UINT32_MAX) continue;
                    e.parent = a;
                    e.child = b;
                    arg_edges[i] = e;
                    i++;
                }
            }

            uint8_t *result = check_edges(arg_edges, outbound);

            std::string found = "exists\n", nfound = "doesn't exist\n";
            std::cout << result[0];
            for (i = 1; i <= result[0]; i++) {
                std::cout << "Edge " << arg_edges[i - 1].parent << " "
                          << arg_edges[i - 1].child << " ";
                if (result[i])
                    std::cout << found;
                else
                    std::cout << nfound;
            }
            delete[] result;
            break;
        }

        case 3: {
            uint16_t i = 0;
            std::cout << "Insert the vertices for which you want to get the in "
                         "and out degree for\n";
            uint32_t arg_vertices[MAX_OPERATON_BUFFER];
            std::fill(arg_vertices, arg_vertices + MAX_OPERATON_BUFFER,
                      UINT32_MAX);
            uint32_t v;
            std::string sv;
            while (true) {
                std::cin >> sv;
                if (sv == "confirm") {
                    break;
                }
                v = s2i(sv);
                if (v == UINT32_MAX) continue;
                arg_vertices[i] = v;
                i++;
            }

            uint32_t *indegree = get_degree(inbound, arg_vertices);
            uint32_t *outdegree = get_degree(outbound, arg_vertices);
            uint16_t sz = indegree[0];  // repurpose v to be size of lhe list of
                                        // arg_vertices

            for (i = 1; i <= sz; i++)
                std::cout << "vertex " << arg_vertices[i - 1] << ": in degree "
                          << indegree[i] << " | outdegree " << outdegree[i]
                          << "\n";
            delete[] indegree;
            delete[] outdegree;
            break;
        }
    }
}
>>>>>>> new

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
    print_menu();
    choose_option(inbound, outbound, costs, vertices, edges);

    // ///////////////////END OF MAIN LOOP///////////////////////// //

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
