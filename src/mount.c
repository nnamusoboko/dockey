#include <stdio.h>
#include <stdlib.h>
#include <sys/mount.h>

#include "mount.h"

static void pdie(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
} 

int setup_mounts(const struct container_config *config) {
    (void)config;

    /*
     * Make mount propagation private so changes in this namespace
     * do  not propagate back to the host mount tree
     * */

    if (mount(NULL, "/", NULL, MS_REC | MS_PRIVATE, NULL) < 0) {
        pdie("mount /proc");
    }

    /*
     * Mount a fresh procfs inside this mount namespace
     * This makes /proc reflect the child PID namespace
     * */
    if (mount("proc", "/proc", "proc", 0, NULL) < 0) {
        pdie("mount /proc/");
    }

    return 0;
}
