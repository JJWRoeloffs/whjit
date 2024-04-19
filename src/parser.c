#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "utils.h"

typedef enum { CH_TAB = '\t', CH_LF = '\n', CH_SPACE = ' ' } Char_Kind;

typedef struct {
    char *buffer;
    size_t len;
    size_t pos;
} Lexer;

Lexer read_file(const char *path) {
    FILE *stream = fopen(path, "r");
    if (stream == NULL)
        finish_err("Could not read input as file");

    char *buffer = NULL;
    size_t len;

    const ssize_t bytes_read = getdelim(&buffer, &len, '\0', stream);
    fclose(stream);
    if (bytes_read <= 0)
        finish_err("Could not read input as file, or file is empty");

    return (Lexer){.buffer = buffer, .len = len};
}

bool is_cmd(const char ch) {
    const char *cmds = "\t \n";
    return strchr(cmds, ch) != NULL;
}

Char_Kind lexer_next(Lexer *l) {
    while (l->pos < l->len && !is_cmd(l->buffer[l->pos])) {
        l->pos++;
    }
    if (l->pos >= l->len)
        return 0;

    return l->buffer[l->pos++];
}

int64_t parse_label(Lexer *l) {
    Char_Kind sign_bit = lexer_next(l);
    if (sign_bit == CH_LF)
        finish_err("Parsing Error. Expected sign bit, got linefeed.");

    int64_t ret = 0;
    Char_Kind current = lexer_next(l);
    if (current == CH_LF)
        finish_err("Parsing Error. Expected value, got linefeed.");

    while (current != CH_LF) {
        ret = ret << 1;
        if (current == CH_TAB)
            ret += 1;
        current = lexer_next(l);
    };

    if (sign_bit == CH_TAB)
        ret = ret * -1;

    return ret;
}

IMP_Kind parse_imp(Lexer *l) {
    switch (lexer_next(l)) {
    case CH_SPACE:
        return IMP_ST;
    case CH_TAB:
        switch (lexer_next(l)) {
        case CH_SPACE:
            return IMP_AR;
        case CH_TAB:
            return IMP_HE;
        case CH_LF:
            return IMP_IO;
        default:
            finish_err("Invalid token found 1");
        }
    case CH_LF:
        return IMP_FL;
    default:
        finish_err("Invalid token found 2");
    }
}

Operator parse_st_operator(Lexer *l) {
    switch (lexer_next(l)) {
    case CH_SPACE:
        return (Operator){.op = OP_PUSH, .param = parse_label(l)};
    case CH_TAB:
        switch (lexer_next(l)) {
        case CH_SPACE:
            return (Operator){.op = OP_COPY, .param = parse_label(l)};
        case CH_LF:
            return (Operator){.op = OP_SLIDE, .param = parse_label(l)};
        case CH_TAB:
        default:
            finish_err("Invalid token found 3");
        }
    case CH_LF:
        switch (lexer_next(l)) {
        case CH_SPACE:
            return (Operator){.op = OP_DUPLICATE};
        case CH_TAB:
            return (Operator){.op = OP_SWAP};
        case CH_LF:
            return (Operator){.op = OP_DISCARD};
        default:
            finish_err("Invalid token found 4");
        }
    default:
        finish_err("Invalid token found 5");
    }
}

Operator parse_ar_operator(Lexer *l) {
    switch (lexer_next(l)) {
    case CH_SPACE:
        switch (lexer_next(l)) {
        case CH_SPACE:
            return (Operator){.op = OP_ADD};
        case CH_TAB:
            return (Operator){.op = OP_SUBSTRACT};
        case CH_LF:
            return (Operator){.op = OP_MULTIPLY};
        default:
            finish_err("Invalid token found 6");
        }
    case CH_TAB:
        switch (lexer_next(l)) {
        case CH_SPACE:
            return (Operator){.op = OP_DIVIDE};
        case CH_TAB:
            return (Operator){.op = OP_MODULO};
        case CH_LF:
        default:
            finish_err("Invalid token found 7");
        }
    case CH_LF:
    default:
        finish_err("Invalid token found 8");
    }
}

Operator parse_he_operator(Lexer *l) {
    switch (lexer_next(l)) {
    case CH_SPACE:
        return (Operator){.op = OP_HEAP_STORE};
    case CH_TAB:
        return (Operator){.op = OP_HEAP_RET};
    case CH_LF:
    default:
        finish_err("Invalid token found 9");
    }
}

Operator parse_io_operator(Lexer *l) {
    switch (lexer_next(l)) {
    case CH_SPACE:
        switch (lexer_next(l)) {
        case CH_SPACE:
            return (Operator){.op = OP_PRCHAR};
        case CH_TAB:
            return (Operator){.op = OP_PRNUM};
        case CH_LF:
        default:
            finish_err("Invalid token found 10");
        }
    case CH_TAB:
        switch (lexer_next(l)) {
        case CH_SPACE:
            return (Operator){.op = OP_INCHAR};
        case CH_TAB:
            return (Operator){.op = OP_INNUM};
        case CH_LF:
        default:
            finish_err("Invalid token found 11");
        }
    case CH_LF:
        finish_err("Invalid token found 12.5");
    default:
        finish_err("Invalid token found 12");
    }
}

Operator parse_fl_operator(Lexer *l) {
    switch (lexer_next(l)) {
    case CH_SPACE:
        switch (lexer_next(l)) {
        case CH_SPACE:
            return (Operator){.op = OP_MARK, .param = parse_label(l)};
        case CH_TAB:
            return (Operator){.op = OP_CALL, .param = parse_label(l)};
        case CH_LF:
            return (Operator){.op = OP_JUMP, .param = parse_label(l)};
        default:
            finish_err("Invalid token found 13");
        }
    case CH_TAB:
        switch (lexer_next(l)) {
        case CH_SPACE:
            return (Operator){.op = OP_CJUMP, .param = parse_label(l)};
        case CH_TAB:
            return (Operator){.op = OP_CNJUMP, .param = parse_label(l)};
        case CH_LF:
            return (Operator){.op = OP_SUBEND};
        default:
            finish_err("Invalid token found 14");
        }
    case CH_LF:
        if (lexer_next(l) != CH_LF)
            finish_err("Program end is [LF][LF][LF], but only two found.");
        return (Operator){.op = OP_EOP};
    default:
        finish_err("Invalid token found 15");
    }
}

// Yes, this could technically be one big switch statement without the IMPs.
// However, the documentation uses IMP, and this is *slightly* more readable.
Operator operator_next(Lexer *l) {
    switch (parse_imp(l)) {
    case IMP_ST:
        return parse_st_operator(l);
    case IMP_AR:
        return parse_ar_operator(l);
    case IMP_HE:
        return parse_he_operator(l);
    case IMP_IO:
        return parse_io_operator(l);
    case IMP_FL:
        return parse_fl_operator(l);
    default:
        finish_err("Unreachable");
    }
}

Program parse_file(const char *path) {
    Lexer lexer = read_file(path);
    Program program = {0};

    while (true) {
        da_append(&program, operator_next(&lexer));
        if (program.items[program.count - 1].op == OP_EOP)
            break;
    };

    free(lexer.buffer);
    return program;
}
