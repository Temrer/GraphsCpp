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
 * @param inbound      Reference to the inbound adjacency list map.
 * @param outbound     Reference to the outbound adjacency list map.
 * @param costs        Reference to the cost map for edges.
 * @param Vertices     Reference to store the number of vertices.
 * @param Edges        Reference to store the number of edges.
 * @param filename     Name of the file to read from.
 * @param vertex_buffer Buffer size for vertex storage.
 * @param manager      Reference to the ID manager.
 */
void read_data(std::unordered_map<uint32_t, uint32_t *> &inbound,
               std::unordered_map<uint32_t, uint32_t *> &outbound,
               std::unordered_map<std::pair<uint32_t, uint32_t>, uint32_t,
                                  pair_hash> &costs,
               uint32_t &Vertices, uint32_t &Edges, char *filename,
               uint32_t vertex_buffer, IdManager &manager);

/**
 * @brief Writes graph data to a file.
 *
 * @param costs    Reference to the cost map of edges.
 * @param Vertices Number of vertices in the graph.
 * @param Edges    Number of edges in the graph.
 * @param filename Name of the file to write to.
 */
void write_data(std::unordered_map<std::pair<uint32_t, uint32_t>, uint32_t,
                                   pair_hash> &costs,
                uint32_t Vertices, uint32_t Edges, std::string filename);

/**
 * @brief Converts a string to an unsigned 32-bit integer.
 *
 * @param s The input string.
 * @return uint32_t The converted integer or UINT32_MAX on failure.
 */
uint32_t s2i(std::string s);

/**
 * @brief Checks if a value exists within an array.
 *
 * @param arr   Pointer to the array.
 * @param value The value to search for.
 * @param size  The size of the array.
 * @param start The starting index for searching (default is 1).
 * @return uint8_t Returns 1 if found, 0 otherwise.
 */
uint8_t is_inside_array(uint32_t *arr, uint32_t value, uint16_t size,
                        uint32_t start = 1);

/**
 * @brief Verifies the existence of edges in the outbound adjacency list.
 *
 * @param edges   Pointer to the list of edges.
 * @param outbound Reference to the outbound adjacency list.
 * @return uint8_t* Dynamically allocated array indicating edge existence.
 */
uint8_t *check_edges(Edge *edges,
                     std::unordered_map<uint32_t, uint32_t *> &outbound);

/**
 * @brief Retrieves the degree of a set of vertices.
 *
 * @param map             Reference to an adjacency list map.
 * @param list_of_vertices Pointer to the list of vertices.
 * @return uint32_t* Dynamically allocated array containing vertex degrees.
 */
uint32_t *get_degree(std::unordered_map<uint32_t, uint32_t *> &map,
                     uint32_t *list_of_vertices);

/**
 * @brief Retrieves adjacency lists for a set of vertices.
 *
 * @param map             Reference to an adjacency list map.
 * @param list_of_vertices Pointer to the list of vertices.
 * @return vertex_map* Dynamically allocated structure containing adjacency
 * lists.
 */
vertex_map *get_vertices_connections(
    std::unordered_map<uint32_t, uint32_t *> &map, uint32_t *list_of_vertices);

/**
 * @brief Retrieves weights for a list of edges.
 *
 * @param edges    Pointer to the list of edges.
 * @param outbound Reference to the outbound adjacency list.
 * @param costs    Reference to the edge cost map.
 * @return uint16_t* Dynamically allocated array containing edge weights.
 */
uint16_t *get_weights_of_edges(
    Edge *edges, std::unordered_map<uint32_t, uint32_t *> &outbound,
    std::unordered_map<std::pair<uint32_t, uint32_t>, uint32_t, pair_hash>
        &costs);

/**
 * @brief Modifies weights for a list of edges.
 *
 * @param edges    Pointer to the list of edges.
 * @param weights  Pointer to the new weights for the edges.
 * @param outbound Reference to the outbound adjacency list.
 * @param costs    Reference to the edge cost map.
 */
void change_weights_of_edges(Edge *edges, uint32_t *weights,
                             std::unordered_map<uint32_t, uint32_t *> &outbound,
                             std::unordered_map<std::pair<uint32_t, uint32_t>,
                                                uint32_t, pair_hash> &costs);

/**
 * @brief Adds a set of vertices to the graph.
 *
 * @param number_of_vertices Number of vertices to add.
 * @param manager            Reference to the ID manager.
 * @param vertex_buffer      Buffer size for vertex storage.
 * @param outbound           Reference to the outbound adjacency list.
 * @param inbound            Reference to the inbound adjacency list.
 * @return uint32_t* Pointer to newly added vertices.
 */
uint32_t *add_vertices(uint32_t number_of_vertices, IdManager &manager,
                       uint16_t vertex_buffer,
                       std::unordered_map<uint32_t, uint32_t *> &outbound,
                       std::unordered_map<uint32_t, uint32_t *> &inbound);

/**
 * @brief Removes a set of vertices from the graph.
 *
 * @param list_of_vertices Pointer to the list of vertices to remove.
 * @param manager          Reference to the ID manager.
 * @param outbound         Reference to the outbound adjacency list.
 * @param inbound          Reference to the inbound adjacency list.
 * @param costs            Reference to the edge cost map.
 * @return uint32_t* Pointer to removed vertices.
 */
uint32_t *remove_vertices(uint32_t *list_of_vertices, IdManager &manager,
                          std::unordered_map<uint32_t, uint32_t *> &outbound,
                          std::unordered_map<uint32_t, uint32_t *> &inbound,
                          std::unordered_map<std::pair<uint32_t, uint32_t>,
                                             uint32_t, pair_hash> &costs);

/**
 * @brief Adds a set of edges to the graph.
 *
 * @param list_of_edges Pointer to the list of edges to add.
 * @param weights       Pointer to corresponding edge weights.
 * @param vertex_buffer Buffer size for vertex storage.
 * @param outbound      Reference to the outbound adjacency list.
 * @param inbound       Reference to the inbound adjacency list.
 * @param costs         Reference to the edge cost map.
 * @return uint32_t* Pointer to added edges.
 */
uint32_t *add_edges(Edge *list_of_edges, uint32_t *weights,
                    uint16_t vertex_buffer,
                    std::unordered_map<uint32_t, uint32_t *> &outbound,
                    std::unordered_map<uint32_t, uint32_t *> &inbound,
                    std::unordered_map<std::pair<uint32_t, uint32_t>, uint32_t,
                                       pair_hash> &costs);

/**
 * @brief Removes a set of edges from the graph.
 *
 * @param list_of_edges Pointer to the list of edges to remove.
 * @param vertex_buffer Buffer size for vertex storage.
 * @param outbound      Reference to the outbound adjacency list.
 * @param inbound       Reference to the inbound adjacency list.
 * @param costs         Reference to the edge cost map.
 * @return uint32_t* Pointer to removed edges.
 */
uint32_t *remove_edges(Edge *list_of_edges, uint16_t vertex_buffer,
                       std::unordered_map<uint32_t, uint32_t *> &outbound,
                       std::unordered_map<uint32_t, uint32_t *> &inbound,
                       std::unordered_map<std::pair<uint32_t, uint32_t>,
                                          uint32_t, pair_hash> &costs);

#endif  // DATA_H_

