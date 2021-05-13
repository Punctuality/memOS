//
// Created by Lazurin Eugene on 13.05.2021.
//

#include <inttypes.h>
#include <stddef.h>
#include "paging.h"

#define PAGE_SIZE 4096
#define PAGE_COUNT 4096 / 32
#define TODO_PHYS_ADR 1234

struct page_table_entry_t pageDirectory[PAGE_COUNT];

void set_page_table_entry(uint32_t endkernel) {

    for (size_t i = 0; i < PAGE_COUNT * PAGE_SIZE;) {
        struct page_table_entry_t *page = (void*) (endkernel + i);
        page->phys_page_adress = TODO_PHYS_ADR;
        page->avail = 2;
        page->global = 0;
        page->zero = 0;
        page->dirty = 0;
        page->accessed = 1;
        page->cache_disabled = 0;
        page->write_throught = 0;
        page->user_supervisor = 1;
        page->read_write = 1;
        page->present = 0;
        i += sizeof(struct page_table_entry_t);
    }
}
