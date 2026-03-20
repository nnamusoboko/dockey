#include "common.h"
#include "runtime.h"
#include "error.h"
#include <stdio.h>

int run_container(struct container_config *config) {
    (void)config;

    printf("[runtime] container launch requested\n");
    return 0;
}
