//
// Created by Lazurin Eugene on 04.05.2021.
//

#ifndef MEMOS_MEMORY_MAP_H
#define MEMOS_MEMORY_MAP_H

#include <stdint.h>

struct memory_map_entry {
    uint64_t base_address;
    uint64_t region_length;
    uint32_t region_type;
    uint32_t extended_attributes;
};

extern uint8_t memory_region_count;

void print_memory_map(struct memory_map_entry* memoryMap);
#endif //MEMOS_MEMORY_MAP_H
