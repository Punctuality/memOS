//
// Created by Lazurin Eugene on 18.05.2021.
//

#include "include/memory_management.h"

void *get_phys_addr(void *virtual_addr) {
    unsigned long pdindex = (unsigned long) virtual_addr >> 22;
    unsigned long ptindex = (unsigned long) virtual_addr >> 12 & 0x03FF;

    unsigned long *pd = (unsigned long *) 0xFFFFF000;

    unsigned long *pt = ((unsigned long *) 0xFFC00000) + (0x400 * pdindex);

    return (void *) ((pt[ptindex] & ~0xFFF) + ((unsigned long) virtual_addr & 0xFFF));
}

void *kmmap(unsigned int *address, size_t size) {
    return NULL;
}

void *kmalloc(size_t size) {
    return NULL;
}

void kfree(void* page) {
    return;
}

