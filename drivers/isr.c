//
// Created by Lazurin Eugene in suffering on 10.06.2021.
//
#include "screen.h"

typedef struct registers {
    unsigned int ds;
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;
    unsigned int int_no, err_code;
    unsigned int eip, cs, eflags, useresp, ss;
} registers_t;

void isr_handler(registers_t regs) {
    print_d("recieved interrupt: ");
    print_hex(regs.int_no, 0x02);
    print_newline();
}