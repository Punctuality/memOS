//
// Created by Lazurin Eugene in pain on 12.06.2021.
//

#include "include/task.h"

volatile task_t *current_task;

volatile task_t *ready_queue;

unsigned int next_pid = 1;

extern page_directory_t *current_directory;

extern unsigned int read_eip();

void create_thread(void (*f)()) {
    asm volatile("cli");
    task_t *parent_task = (task_t *) current_task;

    page_directory_t *directory = current_directory;

    task_t *new_task = (task_t *) kmalloc(sizeof(task_t));

    new_task->id = next_pid++;
    new_task->esp = kmalloc(4096) + 4096;
    new_task->ebp = 0;
    new_task->eip = (unsigned int) f;
    new_task->page_directory = directory;
    new_task->next = 0;

    task_t *tmp_task = (task_t*) ready_queue;
    while (tmp_task->next)
        tmp_task = tmp_task->next;

    tmp_task->next = new_task;

    if (current_task == parent_task) {
        unsigned int esp;
        asm volatile("mov %%esp, %0" : "=r"(esp));
        unsigned int ebp;
        asm volatile("mov %%ebp, %0" : "=r"(ebp));
        new_task->esp = esp;
        new_task->ebp = ebp;
        asm volatile("sti");
    }
    asm volatile("sti");
}

void tasking_init() {
    asm volatile("cli");

    current_task = ready_queue = (task_t *) kmalloc(sizeof(task_t));
    current_task->id = next_pid++;
    current_task->esp = current_task->ebp = 0;
    current_task->eip = 0;
    current_task->page_directory = current_directory;
    current_task->next = 0;

    asm volatile("sti");
}

void task_switch() {
    if (!current_task)
        return;

    uint32_t esp, ebp, eip;

    asm volatile("mov %%esp, %0" : "=r"(esp));
    asm volatile("mov %%ebp, %0" : "=r"(ebp));

    eip = read_eip();

    if (eip == 0x12345)
        return;

    if (current_task->id == 1)
        current_task->eip = eip;

    if (current_task->id == 4){
        current_task = current_task->next;
        if (!current_task)
            current_task = ready_queue;
        current_task->esp = esp;
    }

    current_task->ebp = ebp;

    current_task = current_task->next;

    if (!current_task)
        current_task = ready_queue;

    eip = current_task->eip;
    esp = current_task->esp;
    ebp = current_task->ebp;

    current_directory = current_task->page_directory;

    asm volatile("cli");
    asm volatile("mov %0, %%ecx" :: "r"(eip));
    asm volatile("mov %0, %%esp" :: "r"(esp));
    asm volatile("mov %0, %%ebp" :: "r"(ebp));
    asm volatile("mov %0, %%cr3" :: "r"(current_directory));
    asm volatile("mov $0x12345, %eax");
    asm volatile("sti");
    asm volatile("jmp *%ecx");
}

int fork() {
    asm volatile("cli");

    task_t *parent_task = (task_t *) current_task;

    page_directory_t *directory = current_directory; // should be (page_directory_t *directory = clone_page_directory(current_directory)

    task_t *new_task = (task_t *) kmalloc(sizeof(task_t));

    new_task->id = next_pid++;
    new_task->esp = new_task->ebp = 0;
    new_task->eip = 0;
    new_task->page_directory = directory;
    new_task->next = 0;

    task_t *tmp_task = (task_t *) ready_queue;
    while (tmp_task->next)
        tmp_task = tmp_task->next;

    tmp_task->next = new_task;

    unsigned int eip = read_eip();

    if (current_task == parent_task) {
        unsigned int esp;
        asm volatile("mov %%esp, %0" : "=r"(esp));
        unsigned int ebp;
        asm volatile("mov %%esp, %0" : "=r"(ebp));
        new_task->esp = esp;
        new_task->ebp = ebp;
        new_task->eip = eip;

        asm volatile("sti");

        return new_task->id;
    } else {
        return 0;
    }
}

void move_stack(void *new_stack_start, unsigned int size) {
    return;
}

int get_pid() {
    return current_task->id;
}
