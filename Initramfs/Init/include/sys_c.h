#ifndef _SYS_C_H
#define _SYS_C_H

#include <linux/fcntl.h>
#include <signal.h>
#include <sys/stat.h>
#include <syscall.h>

#include "helper.h"
#include "sys_c/sys_c.h"

#undef NULL
#define NULL 0

#define FD_STDIN 0
#define FD_STDOUT 1
#define FD_STDERR 2

static inline long sys_c_open(const char *path, long flags, long mode) {
    return sys_c_4(SYS_openat, AT_FDCWD, (long)path, flags, mode);
}

static inline long sys_c_read(long fd, void *buf, long size) {
    return sys_c_3(SYS_read, fd, (long)buf, size);
}

static inline long sys_c_stat(long fd, struct stat *st) {
    return sys_c_2(SYS_fstat, fd, (long)st);
}

static inline long sys_c_close(long fd) {
    return sys_c_1(SYS_close, fd);
}

static inline long sys_c_ioctl(long fd, long a1, long a2) {
    return sys_c_3(SYS_ioctl, fd, a1, a2);
}

static inline void sys_c_exit(int status) {
    sys_c_1(SYS_exit, status);
}

static inline long sys_c_mount(const char *src, const char *target, const char *fstype, long flags, const char *data) {
    return sys_c_5(SYS_mount, (long)src, (long)target, (long)fstype, flags, (long)data);
}

static inline long sys_c_ln(const char *target, const char *linkpath) {
    return sys_c_3(SYS_symlinkat, (long)target, AT_FDCWD, (long)linkpath);
}

static inline long sys_c_mkdir(const char *path, long mode) {
    return sys_c_3(SYS_mkdirat, AT_FDCWD, (long)path, mode);
}

static inline long sys_c_cd(const char *path) {
    return sys_c_1(SYS_chdir, (long)path);
}

// #include <sys/sysmacros.h>
// #define MINOR 0
// EXAMPLE => sys_c_mknod("/dev/md1", S_IFBLK | 0600, makedev(9, 1));
static inline long sys_c_mknod(const char *path, long mode, long dev) {
    return sys_c_4(SYS_mknodat, AT_FDCWD, (long)path, mode, dev);
}

static inline long sys_c_umount(const char *path, long flags) {
    return sys_c_2(SYS_umount2, (long)path, flags);
}

// EXAMPLE => sys_c_chown("/tmp/test", 1000, 1000, 0);
static inline long sys_c_chown(const char *path, long owner, long group, long flag) {
    sys_c_5(SYS_fchownat, AT_FDCWD, (long)path, owner, group, flag);
}

static inline long sys_c_rm(const char *path) {
    sys_c_2(SYS_unlinkat, AT_FDCWD, (long)path);
}

static inline long sys_c_rm_dir(const char *path) {
    sys_c_3(SYS_unlinkat, AT_FDCWD, (long)path, AT_REMOVEDIR);
}

static inline long sys_c_exec(const char *path, char *argv[], char *envp[]) {
    return sys_c_3(SYS_execve, (long)path, (long)argv, (long)envp);
}

static inline long sys_c_chroot(const char *path) {
    return sys_c_1(SYS_chroot, (long)path);
}

static inline long sys_c_clone(long sign) {
    return sys_c_1(SYS_clone, sign);
}

static inline long sys_c_fork() {
    return sys_c_0(SYS_fork);
}

static inline long sys_c_wait(unsigned long pid, int *status, int options) {
    sys_c_3(SYS_wait4, pid, (long)&status, options);
}

#define sys_c_write(fd, v) (__builtin_constant_p(v) ? sys_c_3(SYS_write, fd, (long)(const char *)v, (sizeof(v) - 1)) : sys_c_3(SYS_write, fd, (long)(const char *)v, strlen(v)))

static inline void sys_c_write_number(long fd, long v) {
    char buf[20];
    long_to_str(v, buf);
    sys_c_write(fd, buf);
}

#endif  // _SYS_C_H