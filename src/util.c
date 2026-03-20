#include "common.h"
#include "util.h"
#include "error.h"

void *xmalloc(size_t size) {
    void *ptr = malloc(size);
    if (ptr == NULL) {
        pdie("malloc");
    }
    return ptr;
}
