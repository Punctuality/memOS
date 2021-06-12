//
// Created by Lazurin Eugene on 18.05.2021.
//

#ifndef MEMOS_MEMORY_MANAGEMENT_H
#define MEMOS_MEMORY_MANAGEMENT_H

#include <stddef.h>
#include <stdint.h>

typedef struct free_page_entry {
    struct free_page_entry *next;
    int count;
} fpe;

uint32_t kmalloc_int(unsigned int sz, int align, unsigned int *phys);

uint32_t kmalloc(uint32_t sz);

void memset(unsigned char *dest, unsigned char val, int32_t len);

uint32_t alloc_page_internal(void **, int);

#endif //MEMOS_MEMORY_MANAGEMENT_H
