#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "interpreter.h"
#include "parser.h"
#include "utils.h"

Exit_Type call(Program *program, Stack *stack, size_t *index) {
    Operator op;
    while (true) {
        op = program->tape.items[*index];
        switch (op.op) {
        case OP_PUSH:
            da_append(stack, op.param);
            break;
        case OP_DUPLICATE:
            da_append(stack, stack->items[da_len(stack) - 1]);
            break;
        case OP_COPY:
            da_append(stack, stack->items[op.param]);
            break;
        case OP_SWAP: {
            int64_t fst = da_pop(stack);
            int64_t snd = da_pop(stack);
            da_append(stack, fst);
            da_append(stack, snd);
            break;
        }
        case OP_DISCARD:
            (void)da_pop(stack);
            break;
        case OP_SLIDE: {
            int64_t tmp = da_pop(stack);
            for (int i = 0; i < op.param; i++) {
                (void)da_pop(stack);
            }
            da_append(stack, tmp);
            break;
        }
        case OP_ADD: {
            int64_t fst = da_pop(stack);
            int64_t snd = da_pop(stack);
            da_append(stack, fst + snd);
            break;
        }
        case OP_SUBSTRACT: {
            int64_t fst = da_pop(stack);
            int64_t snd = da_pop(stack);
            da_append(stack, fst - snd);
            break;
        }
        case OP_MULTIPLY: {
            int64_t fst = da_pop(stack);
            int64_t snd = da_pop(stack);
            da_append(stack, fst * snd);
            break;
        }
        case OP_DIVIDE: {
            int64_t fst = da_pop(stack);
            int64_t snd = da_pop(stack);
            da_append(stack, fst / snd);
            break;
        }
        case OP_MODULO: {
            int64_t fst = da_pop(stack);
            int64_t snd = da_pop(stack);
            da_append(stack, fst % snd);
            break;
        }
        case OP_HEAP_STORE:
            finish_err("Unimplemented");
        case OP_HEAP_RET:
            finish_err("Unimplemented");
        case OP_MARK: {
            break;
        }
        case OP_CALL: {
            size_t mark = find_mark(&program->marks, op.param);
            if (call(program, stack, &mark) != EXIT_SUBROUTINE)
                return EXIT_PROGRAM;
            break;
        }
        case OP_JUMP: {
            (*index) = find_mark(&program->marks, op.param);
            continue;
        }
        case OP_CJUMP: {
            if (stack->items[stack->count - 1] == 0) {
                (*index) = find_mark(&program->marks, op.param);
                continue;
            }
            break;
        }
        case OP_CNJUMP: {
            if (stack->items[stack->count - 1] < 0) {
                (*index) = find_mark(&program->marks, op.param);
                continue;
            }
            break;
        }
        case OP_SUBEND:
            return EXIT_SUBROUTINE;
        case OP_EOP:
            return EXIT_PROGRAM;
        case OP_PRCHAR:
            printf("%c", (char)stack->items[stack->count - 1]);
            break;
        case OP_INCHAR: {
            char c;
            scanf("%c", &c);
            da_append(stack, (int64_t)c);
            break;
        }
        case OP_PRNUM:
            printf("%ld", stack->items[stack->count - 1]);
            break;
        case OP_INNUM: {
            int64_t i;
            scanf("%ld", &i);
            da_append(stack, i);
            break;
        }
        };
        (*index)++;
    };
}

void interpret(Program *program) {
    Stack stack = {0};
    size_t i = 0;
    if (call(program, &stack, &i) != EXIT_PROGRAM)
        finish_err("Program returned from subroutine without being in one");
}
