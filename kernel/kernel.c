#include "../drivers/keyboard.h"
#include "../drivers/descriptor_tables.h"
#include "shell.h"
#include "memory/include/paging.h"
#include "multiboot.h"
#include "threading/include/task.h"
#include "../drivers/timer.h"
#include "../drivers/screen.h"
#include <stdint.h>


extern void keyboard_handler_int();

unsigned int initial_esp;


void th_loop(char* msg) {
    for (;;) {


        for (int i = 0; i < 10; i ++) {
            puts(msg);
        }
        puts("\n");

        for (i = 0; i < 100000000; i++);

    }
}


void th1() {
    th_loop("th1");
}

void th2() {
    th_loop("th2");
}

void kmain(struct multiboot_info *info, uint32_t initialStack) {

    initial_esp = initialStack;

    init_descriptor_tables();

    load_idt_entry(0x21, (unsigned long) keyboard_handler_int, 0x08, 0x8e);
    kb_init();

    set_page_dir();

    tasking_init();

    shell_init();

    asm volatile("sti");
    init_timer(50);

    asm volatile ("int $0x0");
    asm volatile ("int $0x1");
    asm volatile ("int $0x4");

    int ret = fork();

    print_d("fork() returned: ");
    print_hex_d(ret);
    print_d(", and get_pid returned ");
    print_hex_d(get_pid());

    create_thread(&th1);
    create_thread(&th2);

    while(1) __asm__("hlt\n\t");
}
