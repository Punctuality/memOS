#include "../drivers/keyboard.h"
#include "../drivers/descriptor_tables.h"
#include "shell.h"
#include "memory/include/paging.h"
#include "util.h"
#include "multiboot.h"
#include <stdint.h>


extern void keyboard_handler_int();


void initialize(){

    init_descriptor_tables();
    load_idt_entry(0x21, (unsigned long) keyboard_handler_int, 0x08, 0x8e);
    kb_init();
    set_page_dir();
    shell_init();
}

void kmain(struct multiboot_info *info, uint32_t initialStack) {

    initialize();
    asm volatile ("int $0x0");
    asm volatile ("int $0x1");
    asm volatile ("int $0x4");
    while(1) __asm__("hlt\n\t");
}
