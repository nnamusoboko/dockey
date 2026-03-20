#include "../include/common.h"
#include "../include/process.h"

pid_t spwan_container(struct container_config *config) {
    (void)config;
    return -1;
}

int child_entry(void *arg) {
    (void)arg;
    return -1;
}
