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

#include "Structures.h"

// @brief Carefull: need to free the returned memory
Edge *read_edges();

// @brief Carefull: need to free the returned memory
uint32_t *read_ints();

void opt2(std::unordered_map<uint32_t, uint32_t *> &outbound);

void opt3(std::unordered_map<uint32_t, uint32_t *> &outbound,
          std::unordered_map<uint32_t, uint32_t *> &inbound);

void opt4(std::unordered_map<uint32_t, uint32_t *> &outbound);

void opt5(std::unordered_map<uint32_t, uint32_t *> &inbound);

void opt6(std::unordered_map<uint32_t, uint32_t *> &outbound,
          std::unordered_map<std::pair<uint32_t, uint32_t>, uint32_t, pair_hash>
              &costs);

void opt7(std::unordered_map<uint32_t, uint32_t *> &outbound,
          std::unordered_map<std::pair<uint32_t, uint32_t>, uint32_t, pair_hash>
              &costs);

void opt8(std::unordered_map<uint32_t, uint32_t *> &outbound,
          std::unordered_map<uint32_t, uint32_t *> &inbound, IdManager &manager,
          uint16_t vertex_buffer, uint32_t &Vertices);

void opt9(std::unordered_map<uint32_t, uint32_t *> &outbound,
          std::unordered_map<uint32_t, uint32_t *> &inbound,
          std::unordered_map<std::pair<uint32_t, uint32_t>, uint32_t, pair_hash>
              &costs,
          IdManager &manager, uint32_t &Vertices);

void opt10(uint16_t vertex_buffer, uint32_t &Edges,
           std::unordered_map<uint32_t, uint32_t *> &outbound,
           std::unordered_map<uint32_t, uint32_t *> &inbound,
           std::unordered_map<std::pair<uint32_t, uint32_t>, uint32_t,
                              pair_hash> &costs);

void opt11(uint16_t vertex_buffer, uint32_t &Edges,
           std::unordered_map<uint32_t, uint32_t *> &outbound,
           std::unordered_map<uint32_t, uint32_t *> &inbound,
           std::unordered_map<std::pair<uint32_t, uint32_t>, uint32_t,
                              pair_hash> &costs);

void save(std::unordered_map<std::pair<uint32_t, uint32_t>, uint32_t, pair_hash>
              &costs,
          uint32_t Vertices, uint32_t Edges);

int import(std::unordered_map<uint32_t, uint32_t *> &inbound,
           std::unordered_map<uint32_t, uint32_t *> &outbound,
           std::unordered_map<std::pair<uint32_t, uint32_t>, uint32_t,
                              pair_hash> &costs,
           uint32_t &Vertices, uint32_t &Edges, uint32_t vertex_buffer,
           IdManager &manager, char *filename);
#endif  // UIREAD_H_
