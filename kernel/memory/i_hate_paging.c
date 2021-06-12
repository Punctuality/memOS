//
// Created by Lazurin Eugene on 16.05.2021.
//

#include "include/paging.h"

struct page_directory_entry_t pageDirectoryEntry[1024] __attribute__((aligned(4096)));
struct page_table_entry_t first_page_table[1024] __attribute__((aligned(4096)));

extern void loadPageDirectory(unsigned int*);
extern void enablePaging();

void set_page_dir() {
    for (int i = 0; i < 1024; i++) {
        pageDirectoryEntry[i].read_write = 1;
    }
    for (unsigned int i = 0; i < 1024; i++) {
        first_page_table[i].phys_page_address = i * 0x0001;
        first_page_table[i].read_write = 1;
        first_page_table[i].present = 1;
    }

    pageDirectoryEntry[0].page_table_4kb_aligned_address = ((unsigned int) &first_page_table) >> 12 & 0x3FF;
    pageDirectoryEntry[0].read_write = 1;
    pageDirectoryEntry[0].present = 1;

    loadPageDirectory(pageDirectoryEntry);

    enablePaging();

//    for(;;);
}
