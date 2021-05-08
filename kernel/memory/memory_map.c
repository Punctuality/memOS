//
// Created by Lazurin Eugene on 04.05.2021.
//
#include "include/memory_map.h"
#include "../../drivers/screen.h"
#include "../util.h"

void print_memory_map(struct memory_map_entry* memoryMap) {
    print_newline();

    print_d("Memory base: "); print_d(int_to_str(memoryMap->base_address)); print_newline();
    print_d("Region length: "); print_d(int_to_str(memoryMap->region_length)); print_newline();
    print_d("Memory Type: "); print_d(int_to_str(memoryMap->region_type)); print_newline();
    print_d("Memory Attributes: "); print_d(int_to_str(memoryMap->extended_attributes)); print_newline();
}