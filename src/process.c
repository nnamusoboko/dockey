#include "common.h"
#include "process.h"
#include "error.h"
#include <sys/types.h>

pid_t spawn_container(struct container_config *config) {
    pid_t pid;
    
    pid = fork();
    if (pid < 0) {
        pdie("fork");
    }

    if (pid == 0) {
        /* Child process*/
        execvp(config->argv[0], config->argv);
        
        /* 
         * This reached once execvp() fails
         * On success, execvp() never returns
         *
         */
        pdie("execvp"); 
    }
    /* Parent processs gets child PID */
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
