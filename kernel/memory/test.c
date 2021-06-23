#include "include/paging.h"
#include "../../drivers/screen.h"
#include "../util.h"

#define OFFSET 0x22

int test(void* page_start) {
    int x = *(int*)(page_start + OFFSET);
    if (x == 10) {
        return 1;
    } else {
        return 0;
    }
}

void test_paging() {
    void* old_page_start = kmalloc_a(4096); // старая страничка 
    void* new_page_start = kmalloc_a(4096); // новая страничка

    int* old_x = (old_page_start + OFFSET);
    *old_x = 10;

    mem_cpy(new_page_start, old_page_start, 4096); // скопировать из старой странички в новую всю страницу

    void* a = (void*) old_x;
    
    print_hex_d(a);

    struct page pg = get_page_at(a); // инфо о странице которая содержит "x"

    int* newPtr = new_page_start + ((void*)old_x - old_page_start); // поставить в новую страничку с таким же оффсетом как "х" ...
    *newPtr = 5; // число 5

    // print_hex_d(test()); // вычисление, основанное на старом "x" 
    // print_newline();

    // pg.frame = new_page_start; // ???
    // update_page_at(&x, pg);
    
    // // ЮРА: don't forget about invlpg 
    // // всмысле а причем тут TLB?

    // print_hex_d(test()); // вычисление, основанное на новом "x"
    // print_newline();

    // pg.frame = old_page_start; // ???
    // update_page_at(&x, pg);

    // print_hex_d(test()); // вычисление, основанное на старом "x" 
    // print_newline();
}