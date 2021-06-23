//
// Created by Lazurin Eugene on 16.05.2021.
//

#ifndef MEMOS_PAGING_H
#define MEMOS_PAGING_H

#include <stdint.h>
#include "../../../drivers/isr.h"

void set_page_dir();

typedef struct page_directory_entry_t {
    unsigned char present: 1;
    unsigned char read_write: 1;
    unsigned char user_supervisor: 1;
    unsigned char write_through: 1;
    unsigned char cache_disabled: 1;
    unsigned char accessed: 1;
    unsigned char zero: 1;
    unsigned char page_size: 1; // 0 or 4kb
    unsigned char ignored: 1;
    unsigned char avall: 3;
    unsigned int page_table_4kb_aligned_address: 20;
} __attribute((__packed__)) page_directory_t;

typedef struct page {
    unsigned char present: 1;
    unsigned char read_write: 1;
    unsigned char user_supervisor: 1;
    unsigned char write_through: 1;
    unsigned char cache_disabled: 1;
    unsigned char accessed: 1;
    unsigned char dirty: 1;
    unsigned char zero: 1;
    unsigned char global: 1;
    unsigned char avail: 3;
    unsigned int frame: 20;
}__attribute((__packed__)) page_t;

typedef struct page_table {
    page_t pages[1024];
}__attribute__((aligned(4096))) page_table_t;

void alloc_frame(page_t *page, int is_kernel, int is_writable, int index);

void free_frame(page_t *page);

uint32_t kmalloc(uint32_t sz);

uint32_t kmalloc_a(uint32_t sz);

uint32_t kmalloc_ap(uint32_t sz, uint32_t *phys);

uint32_t kmalloc_p(uint32_t sz, uint32_t *phys);

void switch_page_directory(page_directory_t *dir);

page_directory_t *clone_page_directory(page_directory_t *src);

void page_fault(registers_t regs);

struct page get_page_at(void* ptr);

#endif //MEMOS_PAGING_H
