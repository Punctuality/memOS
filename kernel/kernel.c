#include "../drivers/keyboard.h"
#include "../drivers/idt.h"
#include "shell.h"
#include "memory/include/paging.h"
#include "util.h"
#include "multiboot.h"
#include <stdint.h>


extern void keyboard_handler_int();


void initialize(){
    for (int i = 0; i < IDT_SIZE; i++) {
        load_idt_entry(i, 0, 0, 0);
    }
    idt_init();
    load_idt_entry(0x21, (unsigned long) keyboard_handler_int, 0x08, 0x8e);
    kb_init();
    set_page_dir();
    shell_init();
}

void kmain(struct multiboot_info *info, uint32_t initialStack) {

    initialize();

    while(1) __asm__("hlt\n\t");
}
