#ifndef PROCESS_H
#define PROCESS_H

#include "config.h"

struct child_context {
    struct container_config *config;
};

pid_t spwan_container(struct container_config *config);
int child_entry(void *arg);

#endif
