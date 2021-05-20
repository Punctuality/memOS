//
// Created by Lazurin Eugene on 18.05.2021.
//

#ifndef MEMOS_MEMORY_MANAGEMENT_H
#define MEMOS_MEMORY_MANAGEMENT_H

#include <stddef.h>

void *kmmaloc(size_t size);

void *kmmap(unsigned int *address, size_t size);

void kfree(void *page);

#endif //MEMOS_MEMORY_MANAGEMENT_H
