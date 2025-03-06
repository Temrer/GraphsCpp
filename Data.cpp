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

#include <cstdint>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <ostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "Structures.h"
#define MAX_OPERATON_BUFFER 100

void read_data(std::unordered_map<uint32_t, uint32_t *> &inbound,
               std::unordered_map<uint32_t, uint32_t *> &outbound,
               std::unordered_map<std::pair<uint32_t, uint32_t>, uint32_t,
                                  pair_hash> &costs,
               uint32_t &Vertices, uint32_t &Edges, char *filename,
               uint32_t vertex_buffer) {
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

    // after reading the data, store it in a in-out fast,
    // memory-efficient way create arrays for outbound edges we will
    // store in the first cell the size of the array
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
        }
        if (map.find(current_vertex) == map.end()) {
            i++;
            continue;
        }
        uint16_t deg = map[current_vertex][0];
        result[i + 1] = deg;
        i++;
    }
    std::cout << "\n";
    return result;
}
