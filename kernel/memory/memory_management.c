//
// Created by Lazurin Eugene on 18.05.2021.
//

#include "include/memory_management.h"

extern int _end; // end of kernel. I think, it's not work, it is plug
unsigned int placement_address = (unsigned int) &_end;

uint32_t kmalloc_int(unsigned int sz, int align, unsigned int *phys) {
    if (align == 1 && (placement_address & 0xFFFFF000)) {
        placement_address &= 0xFFFFF000;
        placement_address += 0x1000;
    }
    if (phys) {
        *phys = placement_address;
    }
    unsigned int tmp = placement_address;
    placement_address += sz;
    return tmp;
}

uint32_t kmalloc(uint32_t sz) {
    return kmalloc_int(sz, 0, 0);
}

void memset(unsigned char *dest, unsigned char val, int32_t len) {
    unsigned char *temp = (unsigned char *) dest;
    for (; len != 0; len--) *temp++ = val;
}

uint32_t alloc_page_internal(void **where_to, int how_much) {
    return 0;
}
