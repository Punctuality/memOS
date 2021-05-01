//
// Created by Lazurin Eugene on 01.05.2021.
//

#include "include/paging.h"

int read_cr3() {
    int value;
    __asm__("mov %%cr3, %%eax" : "=a"(value));

    return value;
}

//void write_cr3(uint32_t value) {
//    __asm__("mov cr3, rax" : : "a"(value));
//}
