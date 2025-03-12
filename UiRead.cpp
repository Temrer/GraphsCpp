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

#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <string>

#include "Data.h"
#include "Structures.h"
#include "UiRead.h"

Edge *read_edges() {
    uint16_t i = 0;
    Edge *arg_edges =
        reinterpret_cast<Edge *>(malloc(sizeof(Edge) * MAX_OPERATON_BUFFER));
    for (uint32_t k = 0; k <= MAX_OPERATON_BUFFER; k++)
        arg_edges[k] = NULL_EDGE;
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

void opt3(std::unordered_map<uint32_t, uint32_t *> &map) {
    std::cout << "Insert the vertices for which you want to get the in "
                 "and out degree for\n";
    uint32_t *arg_vertices = read_ints();
    uint32_t *result = get_degree(map, arg_vertices);
    std::cout << "Degrees:\n";
    for (uint16_t i = 1; i <= result[0]; i++) {
        std::cout << "Vertex " << arg_vertices[i - 1] << " has degree "
                  << result[i] << "\n";
    }
    free(arg_vertices);
    delete[] result;
}
