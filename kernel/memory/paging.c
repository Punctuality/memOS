//
// Created by Lazurin Eugene on 01.05.2021.
//

#include "include/paging.h"

int32_t read_cr3() {
    int value;
    __asm__("mov %%cr3, %%eax" : "=a"(value));

    return value;
}

void write_cr3(int32_t value) {
    __asm__("mov %%eax, %%cr3" : : "a"(value));
}
