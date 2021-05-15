//
// Created by Lazurin Eugene on 13.05.2021.
//

#include <stddef.h>
#include "paging.h"

#define PAGE_SIZE 4096
#define PAGE_TABLE_COUNT (4096 / 32)
#define TODO_PHYS_ADR 1234

struct page_table_entry_t pageDirectory[PAGE_TABLE_COUNT];

int32_t read_cr3() {
    int value;
    __asm__("mov %%cr3, %%eax" : "=a"(value));

    return value;
}

void make_page(void *address) {
    struct page_table_entry_t *page = (void *) address;
    page->phys_page_address = TODO_PHYS_ADR;
    page->avail = 2;
    page->global = 0;
    page->zero = 0;
    page->dirty = 0;
    page->accessed = 1;
    page->cache_disabled = 0;
    page->write_through = 0;
    page->user_supervisor = 1;
    page->read_write = 1;
    page->present = 0;
}

void *set_page_table_entry(void *address) {
    if (((int) address) % PAGE_SIZE == 0 || (int) address / 4096 <= 0) {
        make_page(address);
        return address;
    } else {
        make_page(address - (int) address % 4096);
        return address;
    }
}

void set_page_directory(void *start_address) {
    for (size_t i = 0; i < PAGE_TABLE_COUNT * sizeof(struct page_table_entry_t);) {
        pageDirectory[i / sizeof(struct page_table_entry_t)] =
                *(struct page_table_entry_t *) set_page_table_entry(start_address + i);
        i += sizeof(struct page_table_entry_t);
    }
}

void init_memory() {
    set_page_directory((void *) read_cr3());
}

//TODO alignment of structs      V
//TODO phys address function     X
//TODO end of kernel variable    X
