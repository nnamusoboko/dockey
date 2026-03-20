#include "../include/common.h"
#include "../include/runtime.h"
#include "../include/error.h"
#include <stdio.h>

int run_container(struct container_config *config) {
    (void)config;

    printf("[runtime] container launch requested\n");
    return 0;
}
