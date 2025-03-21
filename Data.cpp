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

// A function that will read the specified file and load it into the
// program
#include <sys/types.h>

#include <chrono>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <ostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "IdManager.h"
#include "Structures.h"
#define MAX_OPERATON_BUFFER 100

void read_data(std::unordered_map<uint32_t, uint32_t *> &inbound,
               std::unordered_map<uint32_t, uint32_t *> &outbound,
               std::unordered_map<std::pair<uint32_t, uint32_t>, uint32_t,
                                  pair_hash> &costs,
               uint32_t &Vertices, uint32_t &Edges, char *filename,
               uint32_t vertex_buffer, IdManager &manager) {
    std::ifstream input(filename);
    if (!input.is_open()) {
        // Raise an error telling that the file was not opened
        // succesfully
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
    uint32_t iparent, ichild;      // Read all data and store it
    std::cout << "Reading from file...\n";
    LinkedList *outboundNode;
    LinkedList *inboundNode;
    for (uint32_t i = 0; i < edges; i++) {
        input >> iparent >> ichild >> cost;
        // increase the out and in size
        parent = get_id(manager, iparent);
        child = get_id(manager, ichild);
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
        if (parent < 2)
            std::cout << parent << " " << child << " " << cost << "\n";
    }

    // after reading the data, store it in a in-out fast,
    // memory-efficient way create arrays for outbound edges we will
    // store in the first cell the size of the occupied array
    // store in the cell after the remaining cells of the array
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
        out[out_size[i] + 1] = vertex_buffer;
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
        in[in_size[i] + 1] = vertex_buffer;
        inbound[i] = in;  // passing the pointer of the array to the map
    }

    // initialise isolated vertices
    for (uint32_t i = manager.map.size(); i < vertices; i++) {
        uint32_t new_vertex = generate_id(manager);

        uint32_t *out = reinterpret_cast<uint32_t *>(
            malloc(sizeof(uint32_t) * (vertex_buffer + 1)));
        uint32_t *in = reinterpret_cast<uint32_t *>(
            malloc(sizeof(uint32_t) * (vertex_buffer + 1)));

        out[vertex_buffer] = vertex_buffer;
        in[vertex_buffer] = vertex_buffer;
        in[0] = 0;   // No inbound edges initially
        out[0] = 0;  // No outbound edges initially

        outbound[new_vertex] = out;
        inbound[new_vertex] = in;
    }

    Vertices = vertices;
    Edges = edges;
    input.close();
    std::cout << "reading finished\n";
}

uint32_t s2i(std::string s) {
    try {
        return std::stoi(s);
    } catch (std::exception &e) {
        std::cout << "input was not a integer\n";
        return UINT32_MAX;
    }
}

uint8_t is_inside_array(uint32_t *arr, uint32_t value, uint16_t size,
                        uint32_t start = 1) {
    for (uint16_t i = start; i <= size; i++) {
        if (arr[i] == value) return 1;
    }
    return 0;
}

uint8_t *check_edges(Edge *edges,
                     std::unordered_map<uint32_t, uint32_t *> &outbound) {
    uint8_t *result = new uint8_t[MAX_OPERATON_BUFFER + 1]{0}, sz = 0;

    uint16_t i = 1;
    while (true) {
        uint32_t parent = edges[i - 1].parent, child = edges[i - 1].child;
        if (i == MAX_OPERATON_BUFFER ||
            parent == NULL_EDGE.parent && child == NULL_EDGE.child) {
            result[0] = sz;
            return result;
        }
        result[i] =
            (outbound.find(parent) != outbound.end() &&
             is_inside_array(outbound[parent], child, outbound[parent][0]));
        sz++;
        i++;
    }
}

uint32_t *get_degree(std::unordered_map<uint32_t, uint32_t *> &map,
                     uint32_t *list_of_vertices) {
    uint32_t *result = new uint32_t[MAX_OPERATON_BUFFER + 1]{0};
    uint16_t i = 0;
    while (i < MAX_OPERATON_BUFFER) {
        uint32_t current_vertex = list_of_vertices[i];
        if (current_vertex == UINT32_MAX) {
            result[0] = i;
            i = MAX_OPERATON_BUFFER;
            continue;
        }
        if (map.find(current_vertex) == map.end()) {
            i++;
            continue;
        }
        uint16_t deg = map[current_vertex][0];
        result[i + 1] = deg;
        i++;
    }
    return result;
}

vertex_map *get_vertices_connections(
    std::unordered_map<uint32_t, uint32_t *> &map, uint32_t *list_of_vertices) {
    auto start_time1 = std::chrono::high_resolution_clock::now();
    vertex_map *result = reinterpret_cast<vertex_map *>(
        malloc(sizeof(vertex_map) * MAX_OPERATON_BUFFER + 1));
    uint16_t i = 0;
    while (i < MAX_OPERATON_BUFFER) {
        uint32_t current_vertex = list_of_vertices[i];
        if (current_vertex == UINT32_MAX) {
            result[0] = vertex_map(i, nullptr);
            i = MAX_OPERATON_BUFFER;
        }
        if (map.find(current_vertex) == map.end()) {
            i++;
            continue;
        }
        result[i + 1] = vertex_map(current_vertex, map[current_vertex]);
        i++;
    }
    auto end_time1 = std::chrono::high_resolution_clock::now();
    auto duration1 = std::chrono::duration_cast<std::chrono::milliseconds>(
        end_time1 - start_time1);

    std::cout << "IIProcess took " << duration1.count()
              << " milliseconds to perform.\n";
    return result;
}

uint16_t *get_weights_of_edges(
    Edge *edges, std::unordered_map<uint32_t, uint32_t *> &outbound,
    std::unordered_map<std::pair<uint32_t, uint32_t>, uint32_t, pair_hash>
        &costs) {
    uint16_t *result = new uint16_t[MAX_OPERATON_BUFFER + 1]{0}, sz = 0;

    uint16_t i = 1;
    while (true) {
        uint32_t parent = edges[i - 1].parent, child = edges[i - 1].child;
        auto asd = costs.find(std::make_pair(parent, child));
        if (i == MAX_OPERATON_BUFFER ||
            (parent == NULL_EDGE.parent && child == NULL_EDGE.child)) {
            result[0] = sz;
            return result;
        }
        if (costs.find(std::make_pair(parent, child)) == costs.end()) {
            i++;
            continue;
        }
        result[i] = costs[std::make_pair(parent, child)];
        sz++;
        i++;
    }
    return result;
}
void change_weights_of_edges(Edge *edges, uint32_t *weights,
                             std::unordered_map<uint32_t, uint32_t *> &outbound,
                             std::unordered_map<std::pair<uint32_t, uint32_t>,
                                                uint32_t, pair_hash> &costs) {
    uint16_t i = 0;

    while (i < MAX_OPERATON_BUFFER) {
        uint32_t parent = edges[i].parent, child = edges[i].child;
        if (parent == NULL_EDGE.parent && child == NULL_EDGE.child) return;
        if (costs.find(std::make_pair(parent, child)) == costs.end()) {
            i++;
            continue;
        }
        costs[std::make_pair(parent, child)] = weights[i];
        i++;
    }
    return;
}

uint32_t *add_vertices(uint32_t number_of_vertices, IdManager &manager,
                       uint16_t vertex_buffer,
                       std::unordered_map<uint32_t, uint32_t *> &outbound,
                       std::unordered_map<uint32_t, uint32_t *> &inbound) {
    uint32_t *result = reinterpret_cast<uint32_t *>(
        malloc(sizeof(uint32_t) * (number_of_vertices + 1)));
    result[0] = number_of_vertices;  // Store the count of added vertices

    for (uint32_t i = 0; i < number_of_vertices; i++) {
        uint32_t new_vertex = generate_id(manager);
        result[i + 1] = new_vertex;

        uint32_t *out = reinterpret_cast<uint32_t *>(
            malloc(sizeof(uint32_t) * (vertex_buffer + 1)));
        uint32_t *in = reinterpret_cast<uint32_t *>(
            malloc(sizeof(uint32_t) * (vertex_buffer + 1)));

        out[0] = 0;  // No outbound edges initially
        out[vertex_buffer] = vertex_buffer;
        in[0] = 0;  // No inbound edges initially
        in[vertex_buffer] = vertex_buffer;

        outbound[new_vertex] = out;
        inbound[new_vertex] = in;
    }
    return result;
}

uint32_t *remove_vertices(uint32_t *list_of_vertices, IdManager &manager,
                          std::unordered_map<uint32_t, uint32_t *> &outbound,
                          std::unordered_map<uint32_t, uint32_t *> &inbound,
                          std::unordered_map<std::pair<uint32_t, uint32_t>,
                                             uint32_t, pair_hash> &costs) {
    uint32_t *result = reinterpret_cast<uint32_t *>(
        malloc(sizeof(uint32_t) * (MAX_OPERATON_BUFFER + 1)));
    result[0] = 0;  // Store the count of removed vertices

    for (uint32_t i = 0; i < MAX_OPERATON_BUFFER; i++) {
        uint32_t vertex = list_of_vertices[i];
        if (vertex == UINT32_MAX) break;

        if (outbound.find(vertex) == outbound.end()) continue;

        uint32_t *out_list = outbound[vertex];
        for (uint32_t j = 1; j <= out_list[0]; j++) {
            uint32_t child = out_list[j];

            uint32_t *in_list = inbound[child];
            for (uint32_t k = 1; k <= in_list[0]; k++) {
                if (in_list[k] == vertex) {
                    in_list[k] = in_list[in_list[0]--];
                    break;
                }
            }

            costs.erase({vertex, child});
        }
        free(out_list);
        outbound.erase(vertex);

        uint32_t *in_list = inbound[vertex];
        for (uint32_t j = 1; j <= in_list[0]; j++) {
            uint32_t parent = in_list[j];

            uint32_t *out_list = outbound[parent];
            for (uint32_t k = 1; k <= out_list[0]; k++) {
                if (out_list[k] == vertex) {
                    out_list[k] = out_list[out_list[0]--];
                    break;
                }
            }
            costs.erase({parent, vertex});
        }
        free(in_list);
        inbound.erase(vertex);
        remove_id(manager, vertex);
        result[++result[0]] = vertex;
    }
    return result;
}
