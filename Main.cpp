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
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <ostream>
#include <string>
#include <unordered_map>

#include "Data.h"
#include "Structures.h"
#include "UiRead.h"

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
--------------------------------------\n\
0.  Exit\n\
--------------------------------------\n";
    std::cout << Menu;
}

int choose_option(std::unordered_map<uint32_t, uint32_t *> &inbound,
                  std::unordered_map<uint32_t, uint32_t *> &outbound,
                  std::unordered_map<std::pair<uint32_t, uint32_t>, uint32_t,
                                     pair_hash> &costs,
                  uint32_t &Vertices, uint32_t &Edges, uint32_t vertex_buffer) {
    int option = 0;
    std::string soption;
    while (!option) {
        std::cin >> soption;
        if (soption == "0") return 1;
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
            opt2(outbound);
            break;
        }

        case 3: {
            opt3(outbound, inbound);
            break;
        }

        case 4: {
            opt4(outbound);
            break;
        }
        case 5: {
            opt5(inbound);
            break;
        }
        case 6: {
            opt6(outbound, costs);
            break;
        }
        case 7: {
            opt7(outbound, costs);
            break;
        }
    }
    return 0;
}

// TODO(Temeraire): create multiple batches for inputs bigger than
// MAX_OPERATON_BUFFER
// TODO(Temeraire): free allocated memory used when initializing vectors
int main(int argc, char **argv) {
    std::cout << argc << " | " << argv;
    auto start_time = std::chrono::high_resolution_clock::now();
    if (argc < 2) {
        std::cout << "No file specified. Exiting...\n";
        return 0;
    }

    // /////////////////////////////////////////////////////////////////
    // Declaring variables /////////////////////////////////////////////
    // /////////////////////////////////////////////////////////////////

    std::unordered_map<uint32_t, uint32_t *> inbound;
    std::unordered_map<uint32_t, uint32_t *> outbound;
    std::unordered_map<std::pair<uint32_t, uint32_t>, uint32_t, pair_hash>
        costs;

    uint32_t vertices, edges;
    int ccond = 0;

    uint32_t vertex_buffer = 10;
    // reading overhead-buffer size for the lists of vertices
    if (argc >= 3) {
        vertex_buffer = std::stoi(argv[2]);
    }

    // /////////////////////////////////////////////////////////////////
    // End of declaring variables //////////////////////////////////////
    // /////////////////////////////////////////////////////////////////

    // /////////////////////////////////////////////////////////////////
    // Start main loop /////////////////////////////////////////////////
    // /////////////////////////////////////////////////////////////////

    read_data(inbound, outbound, costs, vertices, edges, argv[1],
              vertex_buffer);
    while (!ccond) {
        print_menu();
        ccond = choose_option(inbound, outbound, costs, vertices, edges,
                              vertex_buffer);
    }

    // /////////////////////////////////////////////////////////////////
    // END OF MAIN LOOP/////////////////////////////////////////////////
    // /////////////////////////////////////////////////////////////////

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
