#include "../include/common.h"
#include "../include/fs.h"

void die(const char *msg) {
    fprintf(stderr, "error: %s\n", msg);
    exit(EXIT_FAILURE);
}

void pdie(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}
