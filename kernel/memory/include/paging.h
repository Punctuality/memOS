
//
// Created by Lazurin Eugene on 01.05.2021.
//

#ifndef MEMOS_PAGING_H
#define MEMOS_PAGING_H
#include <stdint.h>

int32_t read_cr3();
void write_cr3(int32_t value);

#endif //MEMOS_PAGING_H