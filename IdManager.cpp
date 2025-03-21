#include <cstdint>

#include "IdManager.h"

uint32_t get_id(IdManager &manager, uint32_t vertex) {
    if (manager.map.find(vertex) == manager.map.end()) {
        if (manager.unused_ids.size() == 0) {
            manager.max_vertex++;
            return manager.max_vertex - 1;
        }
        uint32_t id = manager.unused_ids.back();
        manager.unused_ids.pop_back();
        return id;
    }
    return manager.map[vertex];
}

uint32_t generate_id(IdManager &manager) {
    if (manager.unused_ids.size() == 0) {
        manager.max_vertex++;
        return manager.max_vertex - 1;
    }
    uint32_t id = manager.unused_ids.back();
    manager.unused_ids.pop_back();
    return id;
}

void remove_id(IdManager &manager, uint32_t vertex) {
    auto pos = manager.map.find(vertex);
    if (pos == manager.map.end()) {
        // it means that it was not read form the file
        manager.unused_ids.push_back(vertex);
        return;
    }

    manager.unused_ids.push_back(manager.map[vertex]);
    manager.map.erase(pos);
}
