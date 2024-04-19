#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "interpreter.h"
#include "parser.h"

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Please provide one argument: the file to run on");
        exit(1);
    }
    Program program = parse_file(argv[1]);

    interpret(&program);

    return 0;
}
