#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "runtime.h"

static void usage(const char *progname) {
    fprintf(stderr, "Usage: %s run <command> [args...]\n", progname);
}

int main(int argc, char **argv) {
    struct container_config config;

    if (argc < 3) {
        usage(argv[0]);
        return EXIT_FAILURE;
    }

    if (strcmp(argv[1], "run") != 0) {
        usage(argv[0]);
        return EXIT_FAILURE;
    }

    config.hostname = "container";
    config.rootfs = "./rootfs";
    config.argv = &argv[2];
    config.argc = argc -2;

    config.use_pid_ns = true;
    config.use_uts_ns = true;
    config.use_mount_ns = true;

    return run_container(&config);
}
