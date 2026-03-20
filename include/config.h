#ifndef CONFIG_H
#define CONFIG_H

#include "common.h"

struct container_config {
    char *hostname;
    char *rootfs;
    char **argv;
    int argc;

    bool use_pid_ns;
    bool use_uts_ns;
    bool use_mount_ns;    
};

#endif
