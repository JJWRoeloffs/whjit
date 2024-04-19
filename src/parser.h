#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef enum {
    IMP_ST, // Stack manipulation
    IMP_AR, // Arithmetic
    IMP_HE, // Heap Access
    IMP_FL, // Flow Control
    IMP_IO, // I/O
} IMP_Kind;

typedef enum {
    OP_PUSH,       // Push the number onto the stack
    OP_DUPLICATE,  // Duplicate the top item on the stack
    OP_COPY,       // Copy the nth item on the stack onto the top of the stack
    OP_SWAP,       // Swap the top two items on the stack
    OP_DISCARD,    // Discard the top item on the stack
    OP_SLIDE,      // Slide n items off the stack, keeping the top item
    OP_ADD,        // Addition
    OP_SUBSTRACT,  // Subtraction
    OP_MULTIPLY,   // Multiplication
    OP_DIVIDE,     // Integer Division
    OP_MODULO,     // Modulo
    OP_HEAP_STORE, // Store in heap
    OP_HEAP_RET,   // Retrieve from heap
    OP_MARK,       // Mark a location in the program
    OP_CALL,       // Call a subroutine
    OP_JUMP,       // Jump to a label
    OP_CJUMP,      // Jump to a label if the top of the stack is zero
    OP_CNJUMP,     // Jump to a label if the top of the stack is negative
    OP_SUBEND,     // End a subroutine and transfer control back
    OP_EOP,        // End the program
    OP_PRCHAR,     // Output the character at the top of the stack
    OP_INCHAR,     // Output the number at the top of the stack
    OP_PRNUM, // Read a character and place it in the location given by the top
              // of the stack
    OP_INNUM, // Read a number and place it in the location given by the top of
              // the stack
} Operator_Kind;

typedef struct {
    Operator_Kind op;
    int64_t param; // arbitrary-width integers, as long as they're 64-bit
} Operator;

typedef struct {
    Operator *items;
    size_t count;
    size_t capacity;
} Tape;

typedef struct {
    int64_t label;
    size_t location;
} Mark;

typedef struct {
    Mark *items;
    size_t count;
    size_t capacity;
} Marks;

typedef struct {
    Tape tape;
    Marks marks;
} Program;

Tape file_to_tape(const char *path);
size_t find_mark(const Marks *marks, int64_t label);
Marks retrieve_program_marks(const Tape *tape);
Program parse_file(const char *path);
