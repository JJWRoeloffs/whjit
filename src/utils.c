#include "utils.h"

_Noreturn void finish_err(char *msg) {
    fprintf(stderr, "%s", msg);
    exit(EXIT_FAILURE);
}
