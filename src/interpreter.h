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

Exit_Type call(Program *program, Stack *stack, size_t *index);
void interpret(Program *program);
