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

#include <stdio.h>

#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <unordered_map>

#include "Data.h"
#include "Structures.h"
#include "UiRead.h"

Edge *read_edges() {
    uint16_t i = 0;
    Edge *arg_edges =
        reinterpret_cast<Edge *>(malloc(sizeof(Edge) * MAX_OPERATON_BUFFER));
    for (uint32_t k = 0; k < MAX_OPERATON_BUFFER; k++) arg_edges[k] = NULL_EDGE;
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
    return arg_edges;
}

uint32_t *read_ints() {
    uint16_t i = 0;
    uint32_t *arg_vertices = reinterpret_cast<uint32_t *>(
        malloc(sizeof(uint32_t) * MAX_OPERATON_BUFFER));
    std::fill(arg_vertices, arg_vertices + MAX_OPERATON_BUFFER, UINT32_MAX);
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
    return arg_vertices;
}

int file_exists(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file) {
        fclose(file);
        return 1;  // File exists
    }
    return 0;  // File does not exist
}

void opt2(std::unordered_map<uint32_t, uint32_t *> &outbound) {
    std::cout << "Input vertices between you want to check the "
                 "existence of an edge\n";
    Edge *arg_edges = read_edges();
    int i;

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
    free(arg_edges);
    delete[] result;
}

void opt3(std::unordered_map<uint32_t, uint32_t *> &outbound,
          std::unordered_map<uint32_t, uint32_t *> &inbound) {
    std::cout << "Insert the vertices for which you want to get the in and out "
                 "degrees\n";
    uint32_t *arg_vertices = read_ints();
    uint32_t *out_degrees = get_degree(outbound, arg_vertices);
    uint32_t *in_degrees = get_degree(inbound, arg_vertices);

    std::cout << "Degrees:\n";
    for (uint16_t i = 1; i <= out_degrees[0]; i++) {
        std::cout << "Vertex " << arg_vertices[i - 1] << " has out-degree "
                  << out_degrees[i] << " and in-degree " << in_degrees[i]
                  << "\n";
    }

    free(arg_vertices);
    delete[] out_degrees;
    delete[] in_degrees;
}

void opt4(std::unordered_map<uint32_t, uint32_t *> &outbound) {
    std::cout << "Insert the vertices for which you want to get the out "
                 "connections for\n";
    uint32_t *arg_vertices = read_ints();

    vertex_map *outdegree = get_vertices_connections(outbound, arg_vertices);
    uint16_t sz = (uint16_t)outdegree[0].vertex;

    for (uint16_t i = 1; i <= sz; i++) {
        uint32_t *list = outdegree[i].list;
        uint32_t sz1 = list[0];
        std::cout << outdegree[i].vertex << " is connected outwards to: ";
        for (uint16_t j = 1; j <= sz1; j++) std::cout << list[j] << " ";
        std::cout << std::endl;
    }

    free(outdegree);
    free(arg_vertices);
}

void opt5(std::unordered_map<uint32_t, uint32_t *> &inbound) {
    std::cout << "Insert the vertices for which you want to get the in "
                 "connections for\n";
    uint32_t *arg_vertices = read_ints();

    vertex_map *indegree = get_vertices_connections(inbound, arg_vertices);
    uint16_t sz = (uint16_t)indegree[0].vertex;

    for (uint16_t i = 1; i <= sz; i++) {
        uint32_t *list = indegree[i].list;
        uint32_t sz1 = list[0];
        std::cout << indegree[i].vertex << " is connected inwards to: ";
        for (uint16_t j = 1; j <= sz1; j++) std::cout << list[j] << " ";
        std::cout << std::endl;
    }

    free(indegree);
    free(arg_vertices);
}

void opt6(std::unordered_map<uint32_t, uint32_t *> &outbound,
          std::unordered_map<std::pair<uint32_t, uint32_t>, uint32_t, pair_hash>
              &costs) {
    std::cout << "Input vertices between which you want to check the weight of "
                 "the edge\n";
    Edge *arg_edges = read_edges();

    uint16_t *result = get_weights_of_edges(arg_edges, outbound, costs);
    uint16_t sz = result[0];
    for (uint16_t i = 0; i < sz; i++) {
        std::cout << "The weight of the edge (" << arg_edges[i].parent << ", "
                  << arg_edges[i].child << ") is: " << result[i + 1] << "\n";
    }

    free(arg_edges);
    delete[] result;
}

void opt7(std::unordered_map<uint32_t, uint32_t *> &outbound,
          std::unordered_map<std::pair<uint32_t, uint32_t>, uint32_t, pair_hash>
              &costs) {
    std::cout << "Input vertices between which you want to change the weight "
                 "of the edge\n";
    std::string i1, i2, i3;
    uint32_t weights[MAX_OPERATON_BUFFER];
    Edge arg_edges[MAX_OPERATON_BUFFER];
    std::fill(arg_edges, arg_edges + MAX_OPERATON_BUFFER, NULL_EDGE);
    uint16_t i = 0;
    int bl = 1;
    while (bl && i < MAX_OPERATON_BUFFER) {
        std::cin >> i1;
        if (i1 == "confirm") {
            bl = 0;
        }
        if (bl) {
            std::cin >> i2 >> i3;
            Edge e;
            uint32_t a, b, c1;
            a = s2i(i1);
            b = s2i(i2);
            c1 = s2i(i3);
            if (a == UINT32_MAX || b == UINT32_MAX || c1 == UINT32_MAX)
                continue;
            weights[i] = (uint16_t)c1;
            e.parent = a;
            e.child = b;
            arg_edges[i] = e;
            i++;
        }
    }

    change_weights_of_edges(arg_edges, weights, outbound, costs);
}

void opt8(std::unordered_map<uint32_t, uint32_t *> &outbound,
          std::unordered_map<uint32_t, uint32_t *> &inbound, IdManager &manager,
          uint16_t vertex_buffer, uint32_t &Vertices) {
    std::cout << "Insert the number of vertices you want to add\n";
    uint32_t v = UINT32_MAX;
    while (v == UINT32_MAX) {
        std::string v1;
        std::cin >> v1;
        v = s2i(v1);
    }
    uint32_t *result =
        add_vertices(v, manager, vertex_buffer, outbound, inbound);
    std::cout << "Added vertices: ";
    for (uint16_t i = 1; i <= result[0]; i++) std::cout << result[i] << " ";
    std::cout << "\n";
    Vertices += result[0];

    free(result);
}

void opt9(std::unordered_map<uint32_t, uint32_t *> &outbound,
          std::unordered_map<uint32_t, uint32_t *> &inbound,
          std::unordered_map<std::pair<uint32_t, uint32_t>, uint32_t, pair_hash>
              &costs,
          IdManager &manager, uint32_t &Vertices) {
    std::cout << "Insert the vertices you want to remove\n";
    uint32_t *arg_vertices = read_ints();

    uint32_t *result =
        remove_vertices(arg_vertices, manager, outbound, inbound, costs);

    std::cout << "Removed vertices: ";
    for (uint16_t i = 1; i <= result[0]; i++) std::cout << result[i] << " ";
    std::cout << "\n";

    Vertices -= result[0];
    free(arg_vertices);
    free(result);
}

void opt10(uint16_t vertex_buffer, uint32_t &Edges,
           std::unordered_map<uint32_t, uint32_t *> &outbound,
           std::unordered_map<uint32_t, uint32_t *> &inbound,
           std::unordered_map<std::pair<uint32_t, uint32_t>, uint32_t,
                              pair_hash> &costs) {
    std::cout << "Insert the edges you want to add (format: vertex1 vertex2 "
                 "weight), type 'confirm' to finish:\n";

    Edge *arg_edges = new Edge[MAX_OPERATON_BUFFER];
    std::fill(arg_edges, arg_edges + MAX_OPERATON_BUFFER, NULL_EDGE);
    uint32_t *weights = new uint32_t[MAX_OPERATON_BUFFER];
    std::fill(weights, weights + MAX_OPERATON_BUFFER, UINT32_MAX);

    std::string i1, i2, i3;
    uint16_t i = 0;
    int bl = 1;
    while (bl && i < MAX_OPERATON_BUFFER) {
        std::cin >> i1;
        if (i1 == "confirm") {
            bl = 0;
        }
        if (bl) {
            std::cin >> i2 >> i3;
            Edge e;
            uint32_t a, b, c1;
            a = s2i(i1);
            b = s2i(i2);
            c1 = s2i(i3);
            if (a == UINT32_MAX || b == UINT32_MAX || c1 == UINT32_MAX)
                continue;
            weights[i] = c1;
            e.parent = a;
            e.child = b;
            arg_edges[i] = e;
            i++;
        }
    }

    uint32_t *result =
        add_edges(arg_edges, weights, vertex_buffer, outbound, inbound, costs);

    std::cout << "Added edges:\n";
    for (uint16_t i = 1; i <= result[0]; i++) {
        std::cout << "(" << arg_edges[i - 1].parent << ", "
                  << arg_edges[i - 1].child << ") with weight "
                  << weights[i - 1] << "\n";
    }

    Edges += result[0];

    delete[] weights;
    delete[] arg_edges;
    delete[] result;
}

void opt11(uint16_t vertex_buffer, uint32_t &Edges,
           std::unordered_map<uint32_t, uint32_t *> &outbound,
           std::unordered_map<uint32_t, uint32_t *> &inbound,
           std::unordered_map<std::pair<uint32_t, uint32_t>, uint32_t,
                              pair_hash> &costs) {
    std::cout << "Insert the edges you want to remove (format: vertex1 "
                 "vertex2), type 'confirm' to finish:\n";

    Edge *arg_edges = read_edges();

    uint32_t *result =
        remove_edges(arg_edges, vertex_buffer, outbound, inbound, costs);

    std::cout << "Removed edges:\n";
    for (uint16_t i = 1; i <= result[0]; i++) {
        std::cout << "(" << arg_edges[i - 1].parent << ", "
                  << arg_edges[i - 1].child << ")\n";
    }

    Edges -= result[0];
    free(arg_edges);
    delete[] result;
}

void opt14(std::unordered_map<uint32_t, uint32_t *> &map) {
    auto it = map.begin();
    while (it != map.end()) {
        std::cout << it->first << "\n";
        it++;
    }
}

void opt15(std::unordered_map<uint32_t, uint32_t *> &map) {
    uint32_t v = UINT32_MAX;
    std::string vs;
    std::cout << "Input vertex to parse: ";
    while (v == UINT32_MAX) {
        std::cin >> vs;
        v = s2i(vs);
    }
    auto it = map.begin();
    if (it == map.end()) return;
    for (uint16_t i = 1; i <= map[v][0]; i++) std::cout << map[v][i] << " ";
    std::cout << "\n";
}

void opt16(std::unordered_map<uint32_t, uint32_t *> &map) {
    uint32_t v = UINT32_MAX;
    std::string vs;
    std::cout << "Input vertex to parse: ";
    while (v == UINT32_MAX) {
        std::cin >> vs;
        v = s2i(vs);
    }
    auto it = map.begin();
    if (it == map.end()) return;
    for (uint16_t i = 1; i <= map[v][0]; i++) std::cout << map[v][i] << " ";
    std::cout << "\n";
}

void save(std::unordered_map<std::pair<uint32_t, uint32_t>, uint32_t, pair_hash>
              &costs,
          uint32_t Vertices, uint32_t Edges) {
    std::string filename;
    std::cout << "Save As: ";
    std::cin >> filename;
    write_data(costs, Vertices, Edges, filename);
}

int import(std::unordered_map<uint32_t, uint32_t *> &inbound,
           std::unordered_map<uint32_t, uint32_t *> &outbound,
           std::unordered_map<std::pair<uint32_t, uint32_t>, uint32_t,
                              pair_hash> &costs,
           uint32_t &Vertices, uint32_t &Edges, uint32_t vertex_buffer,
           IdManager &manager, char *filename) {
    char tmp_filename[100];
    std::cin >> tmp_filename;
    if (file_exists(tmp_filename)) {
        strcpy(filename, tmp_filename);
        return 2;
    }
    std::cout << "File " << tmp_filename << " does not exist\n";
    return 0;
}
