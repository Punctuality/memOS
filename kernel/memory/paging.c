//
// Created by Lazurin Eugene on 13.05.2021.
//

#include <inttypes.h>
#include <stddef.h>
#include "paging.h"

#define PAGE_SIZE 4096
#define PAGE_COUNT 4096 / 32

extern uint32_t endkernel;

struct page_table_entry_t pageDirectory[PAGE_COUNT];

void set_page_table_entry(struct page_table_entry_t *table, int index) {
    for (size_t i = endkernel; i < endkernel + PAGE_COUNT * PAGE_SIZE; i++) {
        struct page_table_entry_t *jopa = (void*) (table + i);
        if ()
    }
}