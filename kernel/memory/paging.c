//
// Created by Lazurin Eugene on 16.05.2021.
//

#include <stdint.h>
#include "include/paging.h"

uint32_t page_directory[1024] __attribute__((aligned(4096)));

extern void loadPageDirectory(unsigned int *);

extern void enablePaging();

void set_page_dir() {
    for (int i = 0; i < 1024; i++) {
        page_directory[i] = 0x00000002;
    }

    for (unsigned int i = 0; i < 1024; i++) {
        uint32_t page_table[1024] __attribute__((aligned(4096)));
        for (unsigned int j = 0; j < 1024; j++) {
            page_table[j] = (j * 0x1000) | 3;
        }
        page_directory[i] = ((unsigned int) page_table) | 3;
    }

    loadPageDirectory(page_directory);
    enablePaging();

}
