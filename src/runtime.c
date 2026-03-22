#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "runtime.h"
#include "process.h"

static void pdie(const char *msg);

int run_container(struct container_config *config) {
    pid_t child_pid;
    int status;

    child_pid = spawn_container(config);

    if (waitpid(child_pid, &status, 0) < 0) {
        pdie("waitpid");
    }

    if (WIFEXITED(status)) {
        return WEXITSTATUS(status);
    }

    if (WIFSIGNALED(status)) {
        fprintf(stderr, "Child terminated by signal %d\n", WTERMSIG(status));
        return EXIT_FAILURE;
    }

    return EXIT_FAILURE;
}

static void pdie(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
} 
