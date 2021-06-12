//
// Created by Lazurin Eugene in suffering on 11.06.2021.
//

#include "timer.h"
#include "isr.h"
#include "screen.h"

extern void write_port(unsigned short port, unsigned char data);

void outb(unsigned short int port, unsigned char value) {
    asm volatile ("outb %1, %0" : : "dN" (port), "a" (value));
}

unsigned int tick = 0;

static void timer_callback(registers_t regs) {
    tick++;
    print_d("Tick: ");
    print_hex_d(tick);
    print_newline();
}

void init_timer(unsigned int frequency) {
    registers_interrupt_handler(IRQ0, &timer_callback);

    unsigned int divisor = 1193180 / frequency;

    outb(0x43, 0x36);

    unsigned char l = (unsigned char) (divisor & 0xFF);
    unsigned char h = (unsigned char) ((divisor >> 8) & 0xFF);

    outb(0x40, l);
    outb(0x40, h);
}