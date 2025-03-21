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
#ifndef STRUCTURES_H_
#define STRUCTURES_H_

#include <cstddef>
#include <cstdint>
#include <functional>
#include <unordered_map>
#include <utility>
#include <vector>

/**
 * @brief Represents an edge in a graph.
 */
struct Edge {
    uint32_t parent;  ///< The parent node of the edge.
    uint32_t child;   ///< The child node of the edge.
};

/// A constant representing a null edge with maximum unsigned 32-bit values.
const Edge NULL_EDGE = {UINT32_MAX, UINT32_MAX};

/**
 * @brief A simple linked list structure to store edges.
 *
 * This is used to efficiently store inbound and outbound edges
 * in a memory-efficient manner.
 */
struct LinkedList {
    uint32_t value;    ///< The value stored in the node.
    LinkedList *next;  ///< Pointer to the next node in the list.

    /**
     * @brief Constructs a new LinkedList node.
     * @param v The value to store in the node.
     */
    explicit LinkedList(uint32_t v) : value(v), next(nullptr) {}
};

/**
 * @brief A hash function for pairs of values.
 *
 * This struct provides a custom hash function for `std::pair`
 * to be used in unordered data structures like `std::unordered_map` or
 * `std::unordered_set`.
 */
struct pair_hash {
    template <class T1, class T2>
    std::size_t operator()(const std::pair<T1, T2> &p) const {
        auto h1 = std::hash<T1>{}(p.first);
        auto h2 = std::hash<T2>{}(p.second);
        return h1 ^ (h2 << 1);  ///< Combines the hashes of the two elements.
    }
};

/**
 * @brief A structure that allows a more advanced storing of vertices and their
 *adjacency lists
 */
struct vertex_map {
    uint32_t *list;   ///< Pointer to it's adjacency list
    uint32_t vertex;  ///< The vertex

    /**
     * @brief Constructs a vertex_map ellement
     * @param v The vertex
     * @param l It's adjacency list
     */
    explicit vertex_map(uint32_t v, uint32_t *l) : vertex(v), list(l) {}
};

struct IdManager {
    std::unordered_map<uint32_t, uint32_t> map;
    std::vector<uint32_t> unused_ids;
    uint32_t max_vertex=0;
};
#endif  // STRUCTURES_H_
