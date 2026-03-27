#include <stdio.h>
#include <stdlib.h>
#include <sys/mount.h>

#include "mount.h"

static void pdie(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
} 

int make_mounts_private(void) {

    /*
     * Make mount propagation private so changes in this namespace
     * do  not propagate back to the host mount tree
     *
     * Runs before chroot()
     * It changes propagation on real root of this mount namespace
     * */

    if (mount(NULL, "/", NULL, MS_REC | MS_PRIVATE, NULL) < 0) {
        pdie("mount propagation private");
    }
    
    return 0;
}

int mount_procfs(void) {
    /*
     * This runs after chroot(), so /proc is inside the container rootfs
     * Mount a fresh procfs inside this mount namespace
     * */
    if (mount("proc", "/proc", "proc", 0, NULL) < 0) {
        pdie("mount /proc");
    }

    return 0;
}
