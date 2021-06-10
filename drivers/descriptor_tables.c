//
// Created by sdfedorov on 14/01/2021.
//

#include "descriptor_tables.h"

struct idt_pointer
{
    unsigned short limit;
    unsigned int base;
} __attribute__((packed));

struct idt_entry
{
    unsigned short int offset_lowerbits;
    unsigned short int selector;
    unsigned char zero;
    unsigned char flags;
    unsigned short int offset_higherbits;
} __attribute__((packed));

struct idt_entry idt_table[IDT_SIZE];
struct idt_pointer idt_ptr;
gdt_entry_t gdt_entries[5];
gdt_ptr_t gdt_ptr;

extern void write_port(unsigned short port, unsigned char data);
extern void load_idt(void*);
extern void gdt_flush(unsigned int);

static void initialize_idt_pointer() {
    idt_ptr.limit = (sizeof(struct idt_entry) * IDT_SIZE) - 1;
    idt_ptr.base = (unsigned int)idt_table;
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
    write_port(PIC_1_DATA, 0x00);
    write_port(PIC_2_DATA, 0x00);

    /* ICW4 - environment info */
    write_port(PIC_1_DATA, 0x01);
    write_port(PIC_2_DATA, 0x01);
    /* Initialization finished */

    /* mask interrupts */
    write_port(0x21 , 0xff);
    write_port(0xA1 , 0xff);
}

void idt_init() {
    initialize_pic();
    initialize_idt_pointer();
    load_idt(&idt_ptr);
}

static void gdt_init() {
    gdt_ptr.limit = (sizeof (gdt_entry_t) * 5) - 1;
    gdt_ptr.base = (unsigned int) &gdt_entries;

    gdt_set_gate(0, 0, 0, 0, 0);               // null
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);// code
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);// data
    gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);// user mode code
    gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);// user mode data

    gdt_flush((unsigned int)&gdt_ptr);
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

void init_descriptor_tables() {
    idt_init();
    gdt_init();
}