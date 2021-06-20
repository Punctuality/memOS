//
// Created by Lazurin Eugene in suffering on 20.06.2021.
//

#include "include/syscall.h"
#include "../../drivers/isr.h"
#include "../../drivers/screen.h"

static void syscall_handler(registers_t *regs);

static void *syscalls[3] = {
        &print,
        &print_hex,
        &print_char
};

unsigned int num_syscalls = 3;

void syscalls_init() {
    registers_interrupt_handler(0x80, &syscall_handler);
}

void syscall_handler(registers_t *regs) {
    if(regs->eax >= num_syscalls)
        return;

    void *location = syscalls[regs->eax];

    int ret;
    asm volatile (" \
     push %1; \
     push %2; \
     push %3; \
     push %4; \
     push %5; \
     call *%6; \
     pop %%ebx; \
     pop %%ebx; \
     pop %%ebx; \
     pop %%ebx; \
     pop %%ebx; \
   " : "=a" (ret) : "r" (regs->edi), "r" (regs->esi), "r" (regs->edx), "r" (regs->ecx), "r" (regs->ebx), "r" (location));
    regs->eax = ret;
}

DEFN_SYSCALL1(print, 0, const char*);
DEFN_SYSCALL1(print_hex, 1, const char*);
DEFN_SYSCALL1(print_char, 2, const char*);