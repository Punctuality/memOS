#include "include/paging.h"
#include "../../drivers/isr.h"
#include "../../drivers/screen.h"
#include "../util.h"

#define OFFSET 0x22

int test(int* value_ref) {
    switch (*value_ref)
    {
    case 10: return 0x1; break;
    case 05: return 0x0; break;
    default: return 0xF; break;
    }
}

void wrong_opt(registers_t regs) {
    print_hex_d(regs.int_no);
}

void test_paging() {

    registers_interrupt_handler(6, wrong_opt);

    void* old_page_start = kmalloc_a(4096); // старая страничка 
    void* new_page_start = kmalloc_a(4096); // новая страничка

    mem_cpy(new_page_start, old_page_start, 4096); // скопировать из старой странички в новую всю страницу

    int* old_x = (old_page_start + OFFSET);
    *old_x = 10;

    int* new_x = new_page_start + ((void*)old_x - old_page_start); // поставить в новую страничку с таким же оффсетом как "х" ...
    *new_x = 5; // число 5

    struct page old_page = get_page_at(old_x);
    struct page new_page = get_page_at(new_x);

    print_newline();

    print_hex_d(test(old_x)); // вычисление, основанное на старом "x" 
    print_newline();

    print_hex_d(test(new_x)); // вычисление, основанное на новом "x" 
    print_newline();

    old_page.frame = new_page.frame;

    asm volatile("invlpg (%0)" ::"r" (old_x) : "memory");
    asm volatile("invlpg (%0)" ::"r" (new_x) : "memory");

    int b = 1;

    print_hex_d(test(old_x)); // вычисление, основанное на старом "x" (обновленном)
    print_newline();
}