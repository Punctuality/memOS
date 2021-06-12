//
// Created by Lazurin Eugene on 07.06.2021.
//

#ifndef MEMOS_ARRAY_H
#define MEMOS_ARRAY_H
#include <stdint.h>
typedef void *type_t;

typedef char (*lessthan_predicate_t)(type_t, type_t);

typedef struct {
    type_t *array;
    unsigned int size;
    unsigned int max_size;
    lessthan_predicate_t less_than;
} ordered_array_t;

// A standard less than predicate.
char standard_less_than_predicate(type_t a, type_t b);

// Create an ordered array.
ordered_array_t create_ordered_array(unsigned int max_size, lessthan_predicate_t less_than);
ordered_array_t place_ordered_array(void *addr, unsigned int max_size, lessthan_predicate_t less_than);

// Destroy an array
void destroy_ordered_array(ordered_array_t *array);

// Add an item into the array.
void insert_ordered_array(type_t item, ordered_array_t *array);

// Lookup the item at index i.
type_t lookup_ordered_array(uint32_t i, ordered_array_t *array);

// Deletes the item at location i from the array.
void remove_ordered_array(uint32_t i, ordered_array_t *array);

#endif //MEMOS_ARRAY_H
