//
// Created by kain on 13.05.2021.
//

#ifndef MEMOS_PAGING_H
#define MEMOS_PAGING_H


struct page_table_entry_t {
    int phys_page_adress: 20;
    char avail: 3;
    char global: 1;
    char zero: 1;
    char dirty: 1;
    char accessed: 1;
    char cache_disabled: 1;
    char write_throught: 1;
    char user_supervisor: 1;
    char read_write: 1;
    char present: 1;
}__attribute((__packed__));


void set_page_table_entry(uint32_t endkernel);


#endif //MEMOS_PAGING_H
