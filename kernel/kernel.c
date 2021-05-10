#include "../drivers/keyboard.h"
#include "../drivers/idt.h"
#include "shell.h"
#include "memory/include/paging.h"
#include "boot.h"

extern void keyboard_handler_int();
extern uint32_t endkernel;

void kmain(struct boot_struct_t *bootStruct) {
    for (int i = 0; i < IDT_SIZE; i++) {
        load_idt_entry(i, 0, 0, 0);
    }
    idt_init();
    load_idt_entry(0x21, (unsigned long) keyboard_handler_int, 0x08, 0x8e);
    kb_init();
    paging();
    shell_init();

    while(1) __asm__("hlt\n\t");
}
