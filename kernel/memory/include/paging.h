//
// Created by Lazurin Eugene on 16.05.2021.
//

#ifndef MEMOS_PAGING_H
#define MEMOS_PAGING_H

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
} __attribute((__packed__)) page_dir;

typedef struct page_table_entry_t {
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
    unsigned int phys_page_address: 20;
}__attribute((__packed__)) page_table;

#endif //MEMOS_PAGING_H
