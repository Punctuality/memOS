//
// Created by Lazurin Eugene in pain on 12.06.2021.
//

#ifndef MEMOS_TASK_H
#define MEMOS_TASK_H

#include "../../memory/include/paging.h"

typedef struct task {
    int id;
    unsigned int esp, ebp;
    unsigned int eip;
    page_directory_t *page_directory;
    struct task *next;
    void *arg;
    void (*f)();
} task_t;

void tasking_init();

void task_switch();

int fork();

void create_thread(void(*f)(), void* arg) {

void move_stack(void *new_stack_start, unsigned int size);

int get_pid();

#endif //MEMOS_TASK_H
