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

#ifndef DATA_H_
#define DATA_H_

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

/**
 * @brief Reads graph data from a file and stores it in adjacency lists and cost
 * mappings.
 *
 * @param inbound A map storing inbound edges as lists.
 * @param outbound A map storing outbound edges as lists.
 * @param costs A map storing the cost of each edge.
 * @param Vertices Reference to store the total number of vertices.
 * @param Edges Reference to store the total number of edges.
 * @param filename Name of the file containing graph data.
 * @param vertex_buffer Buffer size for additional storage.
 */
void read_data(std::unordered_map<uint32_t, uint32_t *> &inbound,
               std::unordered_map<uint32_t, uint32_t *> &outbound,
               std::unordered_map<std::pair<uint32_t, uint32_t>, uint32_t,
                                  pair_hash> &costs,
               uint32_t &Vertices, uint32_t &Edges, char *filename,
               uint32_t vertex_buffer, IdManager &manager);

/**
 * @brief Converts a string to an unsigned 32-bit integer.
 *
 * @param s The input string.
 * @return uint32_t The converted integer or UINT32_MAX on failure.
 */
uint32_t s2i(std::string s);

/**
 * @brief Checks if a value is inside an array.
 *
 * @param arr Pointer to the array.
 * @param value The value to search for.
 * @param size The size of the array.
 * @param start The starting index for the search (default is 1).
 * @return uint8_t Returns 1 if found, 0 otherwise.
 */
uint8_t is_inside_array(uint32_t *arr, uint32_t value, uint16_t size,
                        uint32_t start = 1);

/**
 * @brief Checks the existence of edges in the outbound adjacency list.
 *
 * @param edges Pointer to the list of edges.
 * @param outbound A map storing outbound edges.
 * @return uint8_t* A dynamically allocated array indicating edge existence.
 */
uint8_t *check_edges(Edge *edges,
                     std::unordered_map<uint32_t, uint32_t *> &outbound);

/**
 * @brief Retrieves the degree of a list of vertices.
 *
 * @param map The adjacency list map.
 * @param list_of_vertices Pointer to the list of vertices.
 * @return uint32_t* A dynamically allocated array containing degrees.
 */
uint32_t *get_degree(std::unordered_map<uint32_t, uint32_t *> &map,
                     uint32_t *list_of_vertices);

/**
 * @brief Retrieves the connection lists of a list of vertices.
 *
 * @param map The adjacency list map.
 * @param list_of_vertices Pointer to the list of vertices.
 * @return vertex_map* A dynamically allocated array containing degrees.
 */
vertex_map *get_vertices_connections(
    std::unordered_map<uint32_t, uint32_t *> &map, uint32_t *list_of_vertices);

/**
 * @brief Retrieves the weights of a list of edges
 * @param edges The list of edges to be checked
 * @param outbound The map of the outbound adjacency list
 * @param costs The map of edges containing the weights
 * @return uint16_t* A dynamically allocated array containing the weights
 */
uint16_t *get_weights_of_edges(
    Edge *edges, std::unordered_map<uint32_t, uint32_t *> &outbound,
    std::unordered_map<std::pair<uint32_t, uint32_t>, uint32_t, pair_hash>
        &costs);

/**
 * @brief Retrieves the weights of a list of edges
 * @param edges The list of edges to be checked
 * @param newWeights The list of the new weights to be applied (note: each
 * element of newWeights should correspond directly to the element with the
 * same index of *edges list)
 * @param outbound The map of the outbound adjacency list
 * @param costs The map of edges containing the weights
 * @return uint16_t* A dynamically allocated array containing the weights
 */
void change_weights_of_edges(Edge *edges, uint32_t *weights,
                             std::unordered_map<uint32_t, uint32_t *> &outbound,
                             std::unordered_map<std::pair<uint32_t, uint32_t>,
                                                uint32_t, pair_hash> &costs);

uint32_t *add_vertices(uint32_t number_of_vertices, IdManager &manager,
                       uint16_t vertex_buffer,
                       std::unordered_map<uint32_t, uint32_t *> &outbound,
                       std::unordered_map<uint32_t, uint32_t *> &inbound);

uint32_t *remove_vertices(uint32_t *list_of_vertices, IdManager &manager,
                          std::unordered_map<uint32_t, uint32_t *> &outbound,
                          std::unordered_map<uint32_t, uint32_t *> &inbound,
                          std::unordered_map<std::pair<uint32_t, uint32_t>,
                                             uint32_t, pair_hash> &costs);

uint32_t *add_edges(Edge *list_of_edges, uint32_t *weights,
                    uint16_t vertex_buffer,
                    std::unordered_map<uint32_t, uint32_t *> &outbound,
                    std::unordered_map<uint32_t, uint32_t *> &inbound,
                    std::unordered_map<std::pair<uint32_t, uint32_t>, uint32_t,
                                       pair_hash> &costs);

uint32_t *remove_edges(Edge *list_of_edges, uint16_t vertex_buffer,
                       std::unordered_map<uint32_t, uint32_t *> &outbound,
                       std::unordered_map<uint32_t, uint32_t *> &inbound,
                       std::unordered_map<std::pair<uint32_t, uint32_t>,
                                          uint32_t, pair_hash> &costs);

#endif  // DATA_H_
