//
// Created by Lazurin Eugene on 16.05.2021.
//

#include "include/paging.h"
#include "../../drivers/isr.h"
#include "../../drivers/screen.h"
#include "../shell.h"
#include "../../drivers/idt.h"
#include "../threading/include/task.h"

extern void _end;
uintptr_t placement_address = (uintptr_t) &_end;

extern void copy_page_physical(uint32_t, uint32_t);

page_directory_t kernel_page_directory[1024] __attribute__((aligned(4096)));
page_table_t first_page_table;
page_directory_t *current_directory;

uint32_t kmalloc_int(unsigned int sz, int align, unsigned int *phys) {
    if (align == 1 && (placement_address & 0xFFFFF000)) {
        placement_address &= 0xFFFFF000;
        placement_address += 0x1000;
    }
    if (phys) {
        *phys = placement_address;
    }
    unsigned int tmp = placement_address;
    placement_address += sz;
    return tmp;
}

uint32_t kmalloc(uint32_t sz) {
    return kmalloc_int(sz, 0, 0);
}

uint32_t kmalloc_a(uint32_t sz) {
    return kmalloc_int(sz, 1, 0);
}

uint32_t kmalloc_ap(uint32_t sz, uint32_t *phys) {
    return kmalloc_int(sz, 1, phys);
}

uint32_t kmalloc_p(uint32_t sz, uint32_t *phys) {
    return kmalloc_int(sz, 0, phys);
}

void alloc_frame(page_t *page, int is_kernel, int is_writeable, int index) {
    if (page->frame != 0) {
        page->present = 1;
        page->read_write = (is_writeable == 1) ? 1 : 0;
        page->user_supervisor = (is_kernel == 1) ? 0 : 1;
        return;
    } else {
        page->present = 1;
        page->read_write = (is_writeable == 1) ? 1 : 0;
        page->user_supervisor = (is_kernel == 1) ? 0 : 1;
        page->frame = index * 0x001;
    }
}

void free_frame(page_t *page) {
    page->frame = 0x0;
}

void switch_page_directory(page_directory_t *dir) {
    current_directory = dir;
    asm volatile ("mov %0, %%cr3":: "r"(dir));
    uint32_t cr0;
    asm volatile ("mov %%cr0, %0": "=r"(cr0));
    cr0 |= 0x80000000;
    asm volatile ("mov %0, %%cr0":: "r"(cr0));
}

static page_table_t *clone_table(page_table_t *src) {
    page_table_t *table = (page_table_t*)kmalloc(sizeof(page_table_t));

    for (int i = 0; i < 1024; i++) {
        if (src->pages[i].frame) {
            if (src->pages[i].present) table->pages[i].present = 1;
            if (src->pages[i].read_write) table->pages[i].read_write = 1;
            if (src->pages[i].user_supervisor) table->pages[i].user_supervisor = 1;
            if (src->pages[i].accessed) table->pages[i].accessed = 1;
            if (src->pages[i].dirty) table->pages[i].dirty = 1;
            copy_page_physical(src->pages[i].frame*0x1000, table->pages[i].frame*0x1000);
        }
    }
    return table;
}

page_directory_t *clone_page_directory(page_directory_t src[1024]) {
    page_directory_t *dir = (page_directory_t*)kmalloc(sizeof(page_directory_t[1024]));

    for (int i = 0; i < 1024; i++) {
        if (!src[i].page_table_4kb_aligned_address)
            continue;

        // It's in the kernel, use same pointer
        if (kernel_page_directory[i].page_table_4kb_aligned_address == src[i].page_table_4kb_aligned_address) {
            dir[i] = src[i];
        }
        else {
            page_table_t table = *(page_table_t*) src[i].page_table_4kb_aligned_address;
            dir[i].page_table_4kb_aligned_address = (unsigned int) clone_table(&table) >>12 & 0x3FF;
        }
    }
    return dir;
}

void kek(){
    return;
}

void set_page_dir() {
    for (unsigned int i = 0; i < 1024; i++) {
        alloc_frame(&first_page_table.pages[i], 1, 1, i);
    }

    registers_interrupt_handler(14, page_fault);
    registers_interrupt_handler(13, kek);
    registers_interrupt_handler(6, kek);
    kernel_page_directory[0].page_table_4kb_aligned_address = ((unsigned int) &first_page_table) >> 12 & 0x3FF;
    kernel_page_directory[0].read_write = 1;
    kernel_page_directory[0].present = 1;

    current_directory = clone_page_directory(kernel_page_directory);
    switch_page_directory(kernel_page_directory);

//    for(;;);
}


void page_fault(registers_t regs) {
    asm volatile("cli");
    unsigned int fault_address;
    asm volatile("mov %%cr2, %0" :"=r" (fault_address));

    int present = !(regs.err_code & 0x1);
    int rw = regs.err_code & 0x2;
    int us = regs.err_code & 0x4;
    int reserved = regs.err_code & 0x8;
    int id = regs.err_code & 0x10;

    print_newline();
    print_d("Page fault! ( ");
    if (present) print_d("present ");
    if (rw) print_d("read-only ");
    if (us) print_d("user mode ");
    if (reserved) print_d("reserved");
    print_d(") at 0x");
    print_hex_d(fault_address);
    print_d(" - EIP: ");
    print_hex_d(regs.eip);
    print_newline();
    print_d("PID: ");
    print_hex_d(get_pid());
    panic("PAGE FAULT!!!");
    asm volatile("sti");
}
