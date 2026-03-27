#ifndef MOUNT_H
#define MOUNT_H

int make_mounts_private(void); // pre-chroot
int mount_procfs(void);        // post-chroot

#endif
