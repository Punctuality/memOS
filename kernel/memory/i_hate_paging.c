//
// Created by Lazurin Eugene on 16.05.2021.
//

#include <stdint.h>
#include "i_hate_paging.h"

uint32_t page_directory[1024] __attribute__((aligned(4096)));

extern void loadPageDirectory(unsigned int*);
extern void enablePaging();

void set_page_dir() {
    for (int i = 0; i < 1024; i++) {
        page_directory[i] = 0x00000002;
    }
    uint32_t first_page_table[1024] __attribute__((aligned(4096)));
    for (unsigned int i = 0; i < 1024; i++) {
        first_page_table[i] = (i * 0x1000) | 3;
    }
    page_directory[0] = ((unsigned int)first_page_table) | 3;

    loadPageDirectory(page_directory);
    enablePaging();

}
