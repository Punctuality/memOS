//
// Created by Lazurin Eugene on 07.06.2021.
//

#include "include/array.h"
#include "include/memory_management.h"
#include "../shell.h"

char standard_less_than_predicate(type_t a, type_t b) {
    return (a < b) ? 1 : 0;
}

ordered_array_t create_ordered_array(unsigned int max_size, lessthan_predicate_t less_than) {
    ordered_array_t to_ret;
    to_ret.array = (void *) kmalloc(max_size * sizeof(type_t));
    memset(to_ret.array, 0, max_size * sizeof(type_t));
    to_ret.size = 0;
    to_ret.max_size = max_size;
    to_ret.less_than = less_than;
    return to_ret;
}

ordered_array_t place_ordered_array(void *addr, uint32_t max_size, lessthan_predicate_t less_than) {
    ordered_array_t to_ret;
    to_ret.array = (type_t *) addr;
    memset(to_ret.array, 0, max_size * sizeof(type_t));
    to_ret.size = 0;
    to_ret.max_size = max_size;
    to_ret.less_than = less_than;
    return to_ret;
}

void destroy_ordered_array(ordered_array_t *array) {
//    kfree(array->array);
}

void insert_ordered_array(type_t item, ordered_array_t *array) {
    array->less_than ? (void) 0 : PANIC("ATTEEEEENTION");
    uint32_t iterator = 0;
    while (iterator < array->size && array->less_than(array->array[iterator], item))
        iterator++;
    if (iterator == array->size) // just add at the end of the array.
        array->array[array->size++] = item;
    else {
        type_t tmp = array->array[iterator];
        array->array[iterator] = item;
        while (iterator < array->size) {
            iterator++;
            type_t tmp2 = array->array[iterator];
            array->array[iterator] = tmp;
            tmp = tmp2;
        }
        array->size++;
    }
}

type_t lookup_ordered_array(uint32_t i, ordered_array_t *array) {
    i < array->size ? (void) 0 : PANIC("lookup failed");
    return array->array[i];
}

void remove_ordered_array(uint32_t i, ordered_array_t *array) {
    while (i < array->size) {
        array->array[i] = array->array[i + 1];
        i++;
    }
    array->size--;
}
