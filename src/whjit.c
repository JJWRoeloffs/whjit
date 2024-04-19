#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "parser.h"

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Please provide one argument: the file to run on");
        exit(1);
    }
    Program program = parse_file(argv[1]);

    for (size_t i = 0; i < program.count; i++) {
        printf("Program instruction: {Operator: %d, Argument: %ld}\n",
               program.items[i].op, program.items[i].param);
    }

    return 0;
}
