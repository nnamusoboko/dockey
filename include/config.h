#ifndef CONFIG_H
#define CONFIG_H

#include <stdbool.h>
#include <sys/types.h>

struct container_config {
    char *hostname;
    char *rootfs;
    char **argv;
    int argc;

    bool use_pid_ns;
    bool use_uts_ns;
    bool use_mount_ns;
    bool use_user_ns;

    uid_t host_uid;
    gid_t host_gid;
};

#endif
