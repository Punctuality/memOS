//
// Created by Lazurin Eugene on 13.05.2021.
//

#include <inttypes.h>
#include "paging.h"

#define PAGE_SIZE 4096
#define PAGE_COUNT PAGE_SIZE / 32
extern int8_t endkernel;
struct page_table_entry_t pageDirectory[PAGE_COUNT];

void set_page_table_entry(struct page_table_entry_t *table, int index) {

}