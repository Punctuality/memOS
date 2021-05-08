
//
// Created by Lazurin Eugene on 01.05.2021.
//

#ifndef MEMOS_PAGING_H
#define MEMOS_PAGING_H
#include <stdint.h>
uint64_t page_dir_ptr_tab[4] __attribute__((aligned(0x20)));
uint64_t page_dir[512] __attribute__((aligned(0x1000)));

int32_t read_cr3();
void write_cr3(int32_t value);
void paging();

#endif //MEMOS_PAGING_H