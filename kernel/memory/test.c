#include "include/paging.h"
#include "../../drivers/isr.h"
#include "../../drivers/screen.h"
#include "../util.h"

int x;

int test() {
    switch (x)
    {
    case 10: return 0x11; break;
    case 05: return 0x00; break;
    default: return 0xFF; break;
    }
}

void wrong_opt(registers_t regs) {
    print_hex_d(regs.int_no);
}

static inline void __native_flush_tlb_single(unsigned int addr)
{
   asm volatile("invlpg (%0)" ::"r" (addr) : "memory");
}

void test_paging() {

    registers_interrupt_handler(6, wrong_opt);
    x = 10;
    int* old_x = &x;

    void* old_page_start = ((int)old_x) / 4096 * 4096; // старая страничка 
    void* new_page_start = kmalloc_a(4096); // новая страничка

    mem_cpy(new_page_start, old_page_start, 4096); // скопировать из старой странички в новую всю страницу

    // int* old_x = (old_page_start + OFFSET);
    // *old_x = 10;

    int* new_x = new_page_start + ((void*)old_x - old_page_start); // поставить в новую страничку с таким же оффсетом как "х" ...
    *new_x = 5; // число 5

    struct page* old_page = get_page_at(old_x);
    struct page* new_page = get_page_at(new_x);

    print_newline();

    print_d("Computation on OLD X: ");
    print_hex_d(test()); // вычисление, основанное на старом "x" 
    print_newline();

    old_page->frame = new_page->frame;

    __native_flush_tlb_single(old_x);
    __native_flush_tlb_single(new_x);

    print_d("Computation on OLD X (OLD_frame <- NEW_frame): ");
    print_hex_d(test()); // вычисление, основанное на старом "x" (обновленном)
    print_newline();
}