//
// Created by sdfedorov on 14/01/2021.
//

#ifndef OPERATING_SYSTEMS_ITMO_2020_SHELL_H
#define OPERATING_SYSTEMS_ITMO_2020_SHELL_H

#define PANIC(msg) panic(msg);

struct command {
    int cmd_id;
    int arg_num;
    char* spell;
};
void panic(const char *msg);
void shell_init();
void receive_char(char new_char);

#endif //OPERATING_SYSTEMS_ITMO_2020_SHELL_H
