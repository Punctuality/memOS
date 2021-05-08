//
// Created by Lazurin Eugene on 01.05.2021.
//

#include "include/paging.h"

int32_t read_cr3() {
    int value;
    __asm__("mov %%cr3, %%eax" : "=a"(value));

    return value;
}

void write_cr3(int32_t value) {
    __asm__("mov %%eax, %%cr3" : : "a"(value));
}

void paging() {
    page_dir_ptr_tab[0] = (uint64_t)&page_dir | 1; // set the page directory into the PDPT and mark it present
    page_dir[0] = 0b10000011; //Address=0, 2MIB, RW and present
    asm volatile ("movl %%cr4, %%eax;"
                  "bts $5, %%eax;"
                  "movl %%eax, %%cr4" ::: "eax"); // set bit5 in CR4 to enable PAE

    asm volatile ("movl %0, %%cr3" :: "r" (&page_dir_ptr_tab)); // load PDPT into CR3

    asm volatile ("movl %%cr0, %%eax;"
                  "orl $0x80000000, %%eax;"
                  "movl %%eax, %%cr0;" ::: "eax");
}