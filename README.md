# Dockey

A minimal container runtime built from scratch  to understand how containers actually work under the hood.

## What it does

Dockey demonstrates core container concepts using Linux primitives:

- PID namespace → process isolation  
- UTS namespace → hostname isolation  
- Mount namespace → mount isolation  
- `chroot()` → filesystem isolation  
- `/proc` remount → correct process visibility  
- Basic environment sanitization  

It allows you to run a command inside an isolated environment

## Project Structure

```bash
├── include/        # headers (interfaces)
├── src/            # implementation
├── rootfs/         # minimal filesystem for container
├── Makefile
└── README.md
```

## Build

```bash
make clean
make
```

This will generate the binary:

```bash
./dockey
```

## Prepare root filesystem

A minimal root filesystem is required for the container to be functional.

### 1. Create directories

```bash
mkdir -p rootfs/bin
mkdir -p rootfs/lib/x86_64-linux-gnu
mkdir -p rootfs/lib64
mkdir -p rootfs/etc
mkdir -p rootfs/proc
mkdir -p rootfs/root
```

### 2. Copy binaries

```bash
cp /bin/bash rootfs/bin/
cp /bin/ls rootfs/bin/
cp /bin/ps rootfs/bin/
cp /bin/clear rootfs/bin/
cp /bin/rm rootfs/bin/
```

### 3. Copy required libraries

Check dependencies:

```bash
ldd /bin/bash
ldd /bin/ls
ldd /bin/ps
```

Copy the listed libraries into the same paths inside `rootfs/`.

Example:

```bash
cp /lib/x86_64-linux-gnu/libc.so.6 rootfs/lib/x86_64-linux-gnu/
cp /lib/x86_64-linux-gnu/libtinfo.so.6 rootfs/lib/x86_64-linux-gnu/
cp /lib64/ld-linux-x86-64.so.2 rootfs/lib64/
```

(Repeat for any other libraries shown by `ldd`.)

### 4. Minimal identity setup

```bash
echo 'root:x:0:0:root:/root:/bin/bash' > rootfs/etc/passwd
echo 'root:x:0:' > rootfs/etc/group
```

### (Optional) Fix terminal behavior

To make commands like `clear` work:

```bash
mkdir -p rootfs/usr/share
sudo cp -a /usr/share/terminfo rootfs/usr/share/
```

## Run

```bash
sudo ./dockey run /bin/bash
```

You should see:

```bash
container:/#
```

## Verify isolation

Inside the container:

```bash
echo $$        # should be 1
ps             # very few processes
hostname       # should be "container"
ls /           # shows only rootfs contents
```

## Notes

- Requires `sudo` (namespaces need privileges in this setup)
- This is a learning project, not production-safe
- Uses `chroot()` (not full isolation like real container runtimes)
- Root inside container is still root on host (no user namespace yet)

## Why I am building this ?

Out of curiosity about Docker works

I wanted to understand what a container really is, and see how:

- processes are isolated
- `/proc` reflects different realities
- filesystems are switched
- the kernel presents a different view to a process
