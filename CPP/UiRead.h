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

#ifndef UIREAD_H_
#define UIREAD_H_

#include <cstdint>
#include <unordered_map>

#include "Structures.h"

/**
 * @brief Reads edges from input.
 * @return Pointer to a dynamically allocated array of edges.
 * @note The caller is responsible for freeing the allocated memory.
 */
Edge *read_edges();

/**
 * @brief Reads integers from input.
 * @return Pointer to a dynamically allocated array of integers.
 * @note The caller is responsible for freeing the allocated memory.
 */
uint32_t *read_ints();

/**
 * @brief Checks if edges exist between vertices.
 * @param outbound The adjacency list representing outgoing edges.
 */
void opt2(std::unordered_map<uint32_t, uint32_t *> &outbound);

/**
 * @brief Computes in-degree and out-degree of vertices.
 * @param outbound The adjacency list representing outgoing edges.
 * @param inbound The adjacency list representing incoming edges.
 */
void opt3(std::unordered_map<uint32_t, uint32_t *> &outbound,
          std::unordered_map<uint32_t, uint32_t *> &inbound);

/**
 * @brief Retrieves outbound adjacency lists of vertices.
 * @param outbound The adjacency list representing outgoing edges.
 */
void opt4(std::unordered_map<uint32_t, uint32_t *> &outbound);

/**
 * @brief Retrieves inbound adjacency lists of vertices.
 * @param inbound The adjacency list representing incoming edges.
 */
void opt5(std::unordered_map<uint32_t, uint32_t *> &inbound);

/**
 * @brief Retrieves edge weights from the graph.
 * @param outbound The adjacency list representing outgoing edges.
 * @param costs The mapping of edges to their respective weights.
 */
void opt6(std::unordered_map<uint32_t, uint32_t *> &outbound,
          std::unordered_map<std::pair<uint32_t, uint32_t>, uint32_t, pair_hash>
              &costs);

/**
 * @brief Modifies the weights of existing edges.
 * @param outbound The adjacency list representing outgoing edges.
 * @param costs The mapping of edges to their respective weights.
 */
void opt7(std::unordered_map<uint32_t, uint32_t *> &outbound,
          std::unordered_map<std::pair<uint32_t, uint32_t>, uint32_t, pair_hash>
              &costs);

/**
 * @brief Adds new vertices to the graph.
 * @param outbound The adjacency list representing outgoing edges.
 * @param inbound The adjacency list representing incoming edges.
 * @param manager The ID manager handling vertex allocations.
 * @param vertex_buffer Buffer size for adding vertices.
 * @param Vertices The total number of vertices in the graph.
 */
void opt8(std::unordered_map<uint32_t, uint32_t *> &outbound,
          std::unordered_map<uint32_t, uint32_t *> &inbound, IdManager &manager,
          uint16_t vertex_buffer, uint32_t &Vertices);

/**
 * @brief Removes vertices from the graph.
 * @param outbound The adjacency list representing outgoing edges.
 * @param inbound The adjacency list representing incoming edges.
 * @param costs The mapping of edges to their respective weights.
 * @param manager The ID manager handling vertex removals.
 * @param Vertices The total number of vertices in the graph.
 */
void opt9(std::unordered_map<uint32_t, uint32_t *> &outbound,
          std::unordered_map<uint32_t, uint32_t *> &inbound,
          std::unordered_map<std::pair<uint32_t, uint32_t>, uint32_t, pair_hash>
              &costs,
          IdManager &manager, uint32_t &Vertices);

/**
 * @brief Adds new edges to the graph.
 * @param vertex_buffer Buffer size for managing edges.
 * @param Edges The total number of edges in the graph.
 * @param outbound The adjacency list representing outgoing edges.
 * @param inbound The adjacency list representing incoming edges.
 * @param costs The mapping of edges to their respective weights.
 */
void opt10(uint16_t vertex_buffer, uint32_t &Edges,
           std::unordered_map<uint32_t, uint32_t *> &outbound,
           std::unordered_map<uint32_t, uint32_t *> &inbound,
           std::unordered_map<std::pair<uint32_t, uint32_t>, uint32_t,
                              pair_hash> &costs);

/**
 * @brief Removes edges from the graph.
 * @param vertex_buffer Buffer size for managing edges.
 * @param Edges The total number of edges in the graph.
 * @param outbound The adjacency list representing outgoing edges.
 * @param inbound The adjacency list representing incoming edges.
 * @param costs The mapping of edges to their respective weights.
 */
void opt11(uint16_t vertex_buffer, uint32_t &Edges,
           std::unordered_map<uint32_t, uint32_t *> &outbound,
           std::unordered_map<uint32_t, uint32_t *> &inbound,
           std::unordered_map<std::pair<uint32_t, uint32_t>, uint32_t,
                              pair_hash> &costs);

/**
 * @brief Parses inbound adjacency lists of vertices.
 * @param map The adjacency list representing incoming or outgoing edges.
 */
void opt14(std::unordered_map<uint32_t, uint32_t *> &map);

/**
 * @brief Parses inbound adjacency lists of vertices.
 * @param map The adjacency list representing incoming or outgoing edges.
 */
void opt15(std::unordered_map<uint32_t, uint32_t *> &map);

/**
 * @brief Parses inbound adjacency lists of vertices.
 * @param map The adjacency list representing incoming or outgoing edges.
 */
void opt16(std::unordered_map<uint32_t, uint32_t *> &map);

/**
 * @brief Saves a copy of the graph structure.
 * @param costs The mapping of edges to their respective weights.
 * @param Vertices The total number of vertices in the graph.
 * @param Edges The total number of edges in the graph.
 */
void save(std::unordered_map<std::pair<uint32_t, uint32_t>, uint32_t, pair_hash>
              &costs,
          uint32_t Vertices, uint32_t Edges);

/**
 * @brief Imports a graph from a file.
 * @param inbound The adjacency list representing incoming edges.
 * @param outbound The adjacency list representing outgoing edges.
 * @param costs The mapping of edges to their respective weights.
 * @param Vertices The total number of vertices in the graph.
 * @param Edges The total number of edges in the graph.
 * @param vertex_buffer Buffer size for managing vertices.
 * @param manager The ID manager for handling vertex allocations.
 * @param filename The name of the file to import from.
 * @return int Returns 0 on success, or an error code on failure.
 */
int import(std::unordered_map<uint32_t, uint32_t *> &inbound,
           std::unordered_map<uint32_t, uint32_t *> &outbound,
           std::unordered_map<std::pair<uint32_t, uint32_t>, uint32_t,
                              pair_hash> &costs,
           uint32_t &Vertices, uint32_t &Edges, uint32_t vertex_buffer,
           IdManager &manager, char *filename);

#endif  // UIREAD_H_
