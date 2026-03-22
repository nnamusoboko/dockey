#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "fs.h"

static void pdie(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int setup_rootfs(const struct container_config *config) {
    if (chroot(config->rootfs) < 0) {
        pdie("chroot");
    }

    if (chdir("/") < 0) {
        pdie("chdir");
    }

    return 0;
}

