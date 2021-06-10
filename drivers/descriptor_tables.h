//
// Created by sdfedorov on 14/01/2021.
//

#ifndef OPERATING_SYSTEMS_ITMO_2020_IDT_H
#define OPERATING_SYSTEMS_ITMO_2020_IDT_H

#define PIC_1_CTRL 0x20
#define PIC_2_CTRL 0xA0
#define PIC_1_DATA 0x21
#define PIC_2_DATA 0xA1
#define IDT_SIZE 256

struct gdt_entry_struct {
    unsigned short int limit_low;
    unsigned short int base_low;
    unsigned char base_middle;
    unsigned char access;
    unsigned char granularity;
    unsigned char base_high;
} __attribute__((packed));

typedef struct gdt_entry_struct gdt_entry_t;

struct gdt_ptr_struct {
    unsigned short int limit;
    unsigned int base;
} __attribute__((packed));

typedef struct gdt_ptr_struct gdt_ptr_t;

static void gdt_init();

static void gdt_set_gate(int, unsigned int, unsigned int, unsigned char, unsigned char);

void init_descriptor_tables();

void load_idt_entry(int isr_number, unsigned long base, short int selector, unsigned char flags);

void idt_init();

extern void isr0();

extern void isr1();

extern void isr2();

extern void isr3();

extern void isr4();

extern void isr5();

extern void isr6();

extern void isr7();

extern void isr8();

extern void isr9();

extern void isr10();

extern void isr11();

extern void isr12();

extern void isr13();

extern void isr14();

extern void isr15();

extern void isr16();

extern void isr17();

extern void isr18();

extern void isr19();

extern void isr20();

extern void isr21();

extern void isr22();

extern void isr23();

extern void isr24();

extern void isr25();

extern void isr26();

extern void isr27();

extern void isr28();

extern void isr29();

extern void isr30();

extern void isr31();

#endif //OPERATING_SYSTEMS_ITMO_2020_IDT_H
