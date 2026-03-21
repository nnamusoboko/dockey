#include "common.h"
#include "process.h"
#include "error.h"
#include <sys/types.h>

pid_t spawn_container(struct container_config *config) {
    int flags;
    void *stack;
    void *stack_top;
    pid_t pid;
    struct child_context *ctx;

    flags = SIGCHILD;

    if (config->use_pid_ns) {
        flags |= CLONE_NEWPID;
    }

    if (config->use_uts_ns) {
        flags |= CLONE_NEWUTS;
    }

    if (config->use_mount_ns) {
        flags |= CLONE_NEWNS;
    }

    stack = xmalloc(STACK_SIZE);
    stack_top = (char *)stack + STACK_SIZE;

    ctx = xmalloc(sizeof(*ctx));
    ctx->config = config;

    pid = clone(child_entry, stack_top, flags, ctx);
    if (pid < 0) {
        pdie("clone");
    }
    return pid;
}

int child_entry(void *arg) {
    struct child_context *ctx = arg;
    struct container_config *config = ctx->config;

    if (config->use_uts_ns) {
        if (sethostname(config->hostname, strlen(config->hostanme)) < 0) {
            pdie("sethostname");
        }
    }

    execvp(config->argv[0], config->argv);

    /*
     * We only get here if execvp() fails
     * */
    pdie("execvp");
}
