#include "../include/common.h"
#include "../include/util.h"
#include "../include/error.h"

void *xmalloc(size_t size) {
    void *ptr = malloc(size);
    if (ptr == NULL) {
        pdie("malloc");
    }
    return ptr;
}
