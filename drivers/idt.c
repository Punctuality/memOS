//
// Created by sdfedorov on 14/01/2021.
//

#include "idt.h"
#include "../kernel/threading/include/task.h"

struct idt_pointer {
    unsigned short limit;
    unsigned int base;
} __attribute__((packed));

struct idt_entry {
    unsigned short int offset_lowerbits;
    unsigned short int selector;
    unsigned char zero;
    unsigned char flags;
    unsigned short int offset_higherbits;
} __attribute__((packed));

struct idt_entry idt_table[IDT_SIZE];
struct idt_pointer idt_ptr;
gdt_entry_t gdt_entries[6];
gdt_ptr_t gdt_ptr;
tss_entry_t tss_entry;

extern void write_port(unsigned short port, unsigned char data);

extern void idt_flush(unsigned int);

extern void gdt_flush(unsigned int);

extern void tss_flush();

extern void keyboard_handler_int();

static void initialize_idt_pointer() {
    idt_ptr.limit = (sizeof(struct idt_entry) * IDT_SIZE) - 1;
    idt_ptr.base = (unsigned int) idt_table;

    load_idt_entry(0, (unsigned int) isr0, 0x08, 0x8E);
    load_idt_entry(1, (unsigned int) isr1, 0x08, 0x8E);
    load_idt_entry(2, (unsigned int) isr2, 0x08, 0x8E);
    load_idt_entry(3, (unsigned int) isr3, 0x08, 0x8E);
    load_idt_entry(4, (unsigned int) isr4, 0x08, 0x8E);
    load_idt_entry(5, (unsigned int) isr5, 0x08, 0x8E);
    load_idt_entry(6, (unsigned int) isr6, 0x08, 0x8E);
    load_idt_entry(7, (unsigned int) isr7, 0x08, 0x8E);
    load_idt_entry(8, (unsigned int) isr8, 0x08, 0x8E);
    load_idt_entry(9, (unsigned int) isr9, 0x08, 0x8E);
    load_idt_entry(10, (unsigned int) isr10, 0x08, 0x8E);
    load_idt_entry(11, (unsigned int) isr11, 0x08, 0x8E);
    load_idt_entry(12, (unsigned int) isr12, 0x08, 0x8E);
    load_idt_entry(13, (unsigned int) isr13, 0x08, 0x8E);
    load_idt_entry(14, (unsigned int) isr14, 0x08, 0x8E);
    load_idt_entry(15, (unsigned int) isr15, 0x08, 0x8E);
    load_idt_entry(16, (unsigned int) isr16, 0x08, 0x8E);
    load_idt_entry(17, (unsigned int) isr17, 0x08, 0x8E);
    load_idt_entry(18, (unsigned int) isr18, 0x08, 0x8E);
    load_idt_entry(19, (unsigned int) isr19, 0x08, 0x8E);
    load_idt_entry(20, (unsigned int) isr20, 0x08, 0x8E);
    load_idt_entry(21, (unsigned int) isr21, 0x08, 0x8E);
    load_idt_entry(22, (unsigned int) isr22, 0x08, 0x8E);
    load_idt_entry(23, (unsigned int) isr23, 0x08, 0x8E);
    load_idt_entry(24, (unsigned int) isr24, 0x08, 0x8E);
    load_idt_entry(25, (unsigned int) isr25, 0x08, 0x8E);
    load_idt_entry(26, (unsigned int) isr26, 0x08, 0x8E);
    load_idt_entry(27, (unsigned int) isr27, 0x08, 0x8E);
    load_idt_entry(28, (unsigned int) isr28, 0x08, 0x8E);
    load_idt_entry(29, (unsigned int) isr29, 0x08, 0x8E);
    load_idt_entry(30, (unsigned int) isr30, 0x08, 0x8E);
    load_idt_entry(31, (unsigned int) isr31, 0x08, 0x8E);

    load_idt_entry(32, (unsigned int) irq0, 0x08, 0x8e);  // timer
    load_idt_entry(33, (unsigned int) irq1, 0x08, 0x8e);  // keyboard
    load_idt_entry(34, (unsigned int) irq2, 0x08, 0x8e);  // cascade
    load_idt_entry(35, (unsigned int) irq3, 0x08, 0x8e);  // COM2
    load_idt_entry(36, (unsigned int) irq4, 0x08, 0x8e);  // COM1
    load_idt_entry(37, (unsigned int) irq5, 0x08, 0x8e);  // LPT2
    load_idt_entry(38, (unsigned int) irq6, 0x08, 0x8e);  // floppy
    load_idt_entry(39, (unsigned int) irq7, 0x08, 0x8e);  // LPT1
    load_idt_entry(40, (unsigned int) irq8, 0x08, 0x8e);  // CMOS real-time clock
    load_idt_entry(41, (unsigned int) irq9, 0x08, 0x8e);  // free
    load_idt_entry(42, (unsigned int) irq10, 0x08, 0x8e); // free
    load_idt_entry(43, (unsigned int) irq11, 0x08, 0x8e); // free
    load_idt_entry(44, (unsigned int) irq12, 0x08, 0x8e); // ps2 mouse
    load_idt_entry(45, (unsigned int) irq13, 0x08, 0x8e); // FPU
    load_idt_entry(46, (unsigned int) irq14, 0x08, 0x8e); // primary ATA hard disk
    load_idt_entry(47, (unsigned int) irq15, 0x08, 0x8e); // secondary ATA hard disk

    idt_flush((unsigned int) &idt_ptr);
}

void load_idt_entry(int isr_number, unsigned long base, short int selector, unsigned char flags) {
    idt_table[isr_number].offset_lowerbits = base & 0xFFFF;
    idt_table[isr_number].offset_higherbits = (base >> 16) & 0xFFFF;
    idt_table[isr_number].selector = selector;
    idt_table[isr_number].flags = flags;
    idt_table[isr_number].zero = 0;
}

static void initialize_pic() {
    /* ICW1 - begin initialization */
    write_port(PIC_1_CTRL, 0x11);
    write_port(PIC_2_CTRL, 0x11);

    /* ICW2 - remap offset address of idt_table */
    /*
    * In x86 protected mode, we have to remap the PICs beyond 0x20 because
    * Intel have designated the first 32 interrupts as "reserved" for cpu exceptions
    */
    write_port(PIC_1_DATA, 0x20);
    write_port(PIC_2_DATA, 0x28);

    /* ICW3 - setup cascading */
    write_port(PIC_1_DATA, 0x04);
    write_port(PIC_2_DATA, 0x02);

    /* ICW4 - environment info */
    write_port(PIC_1_DATA, 0x01);
    write_port(PIC_2_DATA, 0x01);
    /* Initialization finished */

    /* mask interrupts */
    write_port(0x21, 0x00);
    write_port(0xA1, 0x00);
}

void idt_init() {
    initialize_pic();
    initialize_idt_pointer();
}

static void gdt_init() {
    gdt_ptr.limit = (sizeof(gdt_entry_t) * 6) - 1;
    gdt_ptr.base = (unsigned int) &gdt_entries;

    gdt_set_gate(0, 0, 0, 0, 0);               // null
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);// code
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);// data
    gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);// user mode code
    gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);// user mode data
    write_tss(5, 0x10, 0x0);

    gdt_flush((unsigned int) &gdt_ptr);
    tss_flush();
}

void set_kernel_stack(unsigned int stack) {
    tss_entry.esp0 = stack;
}

static void gdt_set_gate(int num, unsigned int base, unsigned int limit, unsigned char access, unsigned char gran) {
    gdt_entries[num].base_low = (base & 0xFFFF);
    gdt_entries[num].base_middle = (base >> 16) & 0xFF;
    gdt_entries[num].base_high = (base >> 24) & 0xFF;

    gdt_entries[num].limit_low = (limit & 0xFFFF);
    gdt_entries[num].granularity = (limit >> 16) & 0x0F;

    gdt_entries[num].granularity |= gran & 0xF0;
    gdt_entries[num].access = access;
}

static void write_tss(signed int num, unsigned short ss0, unsigned int esp0) {
    unsigned int base = (unsigned int) &tss_entry;
    unsigned int limit = base + sizeof(tss_entry);

    gdt_set_gate(num, base, limit, 0xE9, 0x00);


    tss_entry.ss0 = ss0;
    tss_entry.esp0 = esp0;
    tss_entry.cs = 0x0b;
    tss_entry.ss = tss_entry.ds = tss_entry.es = tss_entry.fs = tss_entry.gs = 0x13;
    tss_entry.iomap_base = sizeof(tss_entry);
}

void init_descriptor_tables() {
    idt_init();
    gdt_init();
}

void set_dir(task_t current_task) {
    tss_entry.cr3 = (unsigned int) current_task.page_directory;
}
