//
// Created by Lazurin Eugene in suffering on 10.06.2021.
//
#include "isr.h"
#include "screen.h"

isr_t interrupt_handlers[256];

extern void write_port(unsigned short port, unsigned char data);

void isr_handler(registers_t regs) {
    print_d("recieved interrupt: ");
    print_hex(regs.int_no, 0x02);
    print_newline();
}

void irq_handler(registers_t regs) {
    if (regs.int_no >= 40) {
        write_port(0xA0, 0x20);
    }
    write_port(0x20, 0x20);

    if (interrupt_handlers[regs.int_no] != 0) {
        isr_t handler = interrupt_handlers[regs.int_no];
        handler(regs);
    }
}

void registers_interrupt_handler(unsigned char n, isr_t handler) {
    interrupt_handlers[n] = handler;
}