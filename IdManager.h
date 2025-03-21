#include <cstdint>

#include "Structures.h"

uint32_t get_id(IdManager &manager, uint32_t vertex);

void remove_id(IdManager &manager, uint32_t vertex);

uint32_t generate_id(IdManager &manager);
