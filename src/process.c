#define _GNU_SOURCE // needed for clone

#include <sched.h>   // gives clone and namespace flags
#include <signal.h>  
#include <stdio.h> 
#include <stdlib.h>
#include <string.h> 
#include <unistd.h>
#include <stddef.h> // like size_t
#include <fcntl.h>
#include <sys/types.h>

#include "process.h"
#include "mount.h"
#include "fs.h"

#define STACK_SIZE (1024 * 1024)

static void pdie(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

static void *xmalloc(size_t size) {
    void *ptr = malloc(size);   
    if (ptr == NULL) {
        pdie("malloc");
    }
    return ptr;
}

static void write_file(const char *path, const char *data) {
    int fd;
    ssize_t written;
    size_t len;

    fd = open(path, O_WRONLY);
    if (fd < 0) {
        pdie("open path");
    }

    len = strlen(data);
    written = write(fd, data, len);
    if (written < 0 || (size_t)written != len) {
        close(fd);
        pdie("write");
    }

    if (close(fd) < 0) {
        pdie("close");
    }
}

static void setup_uid_gid_map(pid_t child_pid, uid_t host_uid, gid_t host_gid) {
    char path[256];
    char map[256];

    /*
     * For unpriveliged GID mapping
     * echo deny > /proc/<pid>/setgroups
     * before writing gid_map
     * */

    snprintf(path, sizeof(path), "/proc/%d/setgroups", child_pid);
    if (access(path, F_OK) == 0) {
        write_file(path, "deny");
    }

    snprintf(path, sizeof(path), "/proc/%d/uid_map", child_pid);
    snprintf(path, sizeof(map), "0 %d 1\n", host_uid);
    write_file(path, map);

    snprintf(path, sizeof(path), "/proc/%d/gid_map", child_pid);
    snprintf(map, sizeof(map), "0 %d 1\n", host_gid);
    write_file(path, map);
}

static  void setup_environment(void) {
    /*
     * Remove host prompt behaviour and shell hooks that make 
     * the container leaky
     * */
    unsetenv("PROMPT_COMMAND");
    unsetenv("PS0");
    unsetenv("PS2");
    unsetenv("PS3");
    unsetenv("PS4");

    // set a few shell environment variables
    if (setenv("PATH", "/bin/:/usr/bin/", 1) < 0) {
        pdie("setenv PATH");
    }

    if (setenv("HOME", "/root", 1) < 0) {
        pdie("setenv HOME");
    }

    if (setenv("USER", "root", 1) < 0) {
        pdie("setenv USER");
    }

    if (setenv("LOGNAME", "root", 1) < 0) {
        pdie("setenv LOGNAME");
    }

    if (setenv("PS1", "container:\\w# ", 1) < 0) {
        pdie("setenv PS1");
    }

    if (setenv("TERM", "xterm-256color", 1) < 0) {
        pdie("setenv TERM");
    }

}

static void wait_for_parent_mapping(int read_fd) {
    char buf;
    ssize_t n;

    n = read(read_fd, &buf, 1);
    if (n < 0) {
        pdie("read sync pipe");
    }

    /*
     * n == 0: parent closed pipe without writing
     * still enough as "continue" signal
     */
    if (close(read_fd) < 0) {
        pdie("close sync read_fd");
    }
} 

int child_entry(void *arg) {
    struct child_context *ctx = arg;
    struct container_config *config = ctx->config;

    if (config->use_user_ns) {
        close(ctx->sync_pipe[1]);
        wait_for_parent_mapping(ctx->sync_pipe[0]);
    } else {
        close(ctx->sync_pipe[0]);
        close(ctx->sync_pipe[1]);
    }

    if (config->use_uts_ns) {
        if (sethostname(config->hostname, strlen(config->hostname)) < 0) {
            pdie("sethostname");
        }
    }

    if (config->use_mount_ns) {
        if (make_mounts_private() < 0) {
            pdie("make_mounts_private");
        }
    }

    if (setup_rootfs(config) < 0) {
        pdie("setup_rootfs");
    }

    if (config->use_mount_ns) {
        if (mount_procfs() < 0) {
            pdie("mount_procfs");
        }
    }

    setup_environment();

    execvp(config->argv[0], config->argv);
    pdie("execvp"); // we only reach here if execvp() fails

    return -1;
}


pid_t spawn_container(struct container_config *config) {
    int flags;
    void *stack;
    void *stack_top;
    pid_t pid;
    struct child_context *ctx;

    flags = SIGCHLD;
    
    ctx = xmalloc(sizeof(*ctx));
    ctx->config- = config;


    if (pipe(ctx->sync_pipe) < 0) {
        pdie("pipe");
    }

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


    pid = clone(child_entry, stack_top, flags, ctx);
    if (pid < 0) {
        pdie("clone");
    }

    if (config->use_user_ns) {
        close(ctx->sync_pipe[0]);

        setup_uid_gid_map(pid, config->host_uid, config->host_gid);

        /* Signal chil that mapping is complete*/
        if (write(ctx->sync_pipe[1], "x", 1) < 0) {
            pdie("write sync pipe");
        }

        if (close(ctx->sync_pipe[1]) < 0) {
            pdie("close sync write_fd");
        }
    } else {
        close(ctx->sync_pipe[0]);
        close(ctx->sync_pipe[1]);
    }

    return pid;
}
