#ifndef PROCESS_H
#define PROCESS_H

#include "config.h"

struct child_context {
    struct container_config *config;
    int sync_pipe[2];
};

pid_t spawn_container(struct container_config *config);
int child_entry(void *arg);

#endif
