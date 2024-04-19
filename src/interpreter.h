#pragma once
#include <stdint.h>

#include "parser.h"

typedef enum {
    EXIT_SUBROUTINE,
    EXIT_PROGRAM,
} Exit_Type;

typedef struct {
    int64_t *items; // arbitrary-width integers, as long as they're 64-bit
    size_t count;
    size_t capacity;
} Stack;

typedef struct {
    int64_t label;
    size_t location;
} Mark;

typedef struct {
    Mark *items;
    size_t count;
    size_t capacity;
} Marks;

size_t find_mark(Marks *marks, int64_t label);
Exit_Type call(Program *program, Stack *stack, Marks *marks, size_t *index);
void interpret(Program *program);
