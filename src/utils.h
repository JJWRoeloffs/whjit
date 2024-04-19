#pragma once

// Implementation of a Dynamic Array, taken largely from tsoding.
// Expects a struct of type {TYPE *items; size_t count; size_t capacity;}
#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

_Noreturn void finish_err(char *msg);

#define DA_INIT_CAP 256

// Append an item to a dynamic array
#define da_append(da, item)                                                    \
    do {                                                                       \
        if ((da)->count >= (da)->capacity) {                                   \
            (da)->capacity =                                                   \
                (da)->capacity == 0 ? DA_INIT_CAP : (da)->capacity * 2;        \
            (da)->items =                                                      \
                realloc((da)->items, (da)->capacity * sizeof(*(da)->items));   \
            if ((da)->items == NULL)                                           \
                finish_err("Ran out of memory");                               \
        }                                                                      \
                                                                               \
        (da)->items[(da)->count++] = (item);                                   \
    } while (0)

#define da_free(da) free((da).items)

// Append several items to a dynamic array
#define da_append_many(da, new_items, new_items_count)                         \
    do {                                                                       \
        if ((da)->count + new_items_count > (da)->capacity) {                  \
            if ((da)->capacity == 0) {                                         \
                (da)->capacity = DA_INIT_CAP;                                  \
            }                                                                  \
            while ((da)->count + new_items_count > (da)->capacity) {           \
                (da)->capacity *= 2;                                           \
            }                                                                  \
            (da)->items =                                                      \
                realloc((da)->items, (da)->capacity * sizeof(*(da)->items));   \
            if ((da)->items == NULL)                                           \
                finish_err("Ran out of memory");                               \
        }                                                                      \
        memcpy((da)->items + (da)->count, new_items,                           \
               new_items_count * sizeof(*(da)->items));                        \
        (da)->count += new_items_count;                                        \
    } while (0)

// Remove the last inserted element from the array, turning it into a stack.
#define da_pop(da) ((da)->items[(da)->count--])
