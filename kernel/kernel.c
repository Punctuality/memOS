#include "../drivers/keyboard.h"
#include "../drivers/idt.h"
#include "shell.h"
#include "memory/i_hate_paging.h"
#include "util.h"
#include "multiboot.h"
#include <stdint.h>


extern void keyboard_handler_int();

char* vptr;
void test(char* s) {
    mem_cpy(vptr++, "\0", 1);
    mem_cpy(vptr++, s, 1);
}

void initialize(){
    vptr = 0xB8000;
    test("A");
    for (int i = 0; i < IDT_SIZE; i++) {
        load_idt_entry(i, 0, 0, 0);
    }
    test("B");
    idt_init();
    test("C");
    load_idt_entry(0x21, (unsigned long) keyboard_handler_int, 0x08, 0x8e);
    test("D");
    kb_init();
    test("E");
    set_page_dir();
    test("F");
    shell_init();
}

void kmain(struct multiboot_info *info, uint32_t initialStack) {

    initialize();

    while(1) __asm__("hlt\n\t");
}
