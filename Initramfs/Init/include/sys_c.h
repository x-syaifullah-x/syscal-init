#ifndef _SYS_C_H
#define _SYS_C_H

#include <sys/fcntl.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <sys/types.h>

#include "helper.h"
#include "sys_c/sys_c.h"

#undef NULL
#define NULL 0

static inline __off_t sys_c_lseek(int __fd, __off_t __offset, int __whence) {
    return sys_c_3(SYS_lseek, __fd, __offset, __whence);
}

static inline int sys_c_init_module(void *__module_image, const unsigned long __len, const char *__param) {
    return sys_c_3(SYS_init_module, (long)__module_image, __len, (long)__param);
}

static inline void *sys_c_mmap(void *__addr, size_t __len, int __prot, int __flags, int __fd, __off_t __offset) {
    return (void *)sys_c_6(SYS_mmap, (long)__addr, __len, __prot, __flags, __fd, __offset);
}

static inline int sys_c_munmap(void *__addr, size_t __len) {
    return sys_c_2(SYS_munmap, (long)__addr, __len);
}

static inline int sys_c_open(const char *__path, int __oflag, mode_t __mode) {
    return sys_c_4(SYS_openat, AT_FDCWD, (long)__path, __oflag, __mode);
}

static inline ssize_t sys_c_read(int __fd, void *__buf, size_t __nbytes) {
    return sys_c_3(SYS_read, __fd, (long)__buf, __nbytes);
}

static inline int sys_c_fstat(int __fd, struct stat *__buf) {
    return sys_c_2(SYS_fstat, __fd, (long)__buf);
}

static inline int sys_c_stat(const char *__restrict __file, struct stat *__restrict __buf) {
    return sys_c_2(SYS_stat, (long)__file, (long)__buf);
}

static inline int sys_c_close(int __fd) {
    return sys_c_1(SYS_close, __fd);
}

static inline int sys_c_ioctl(int __fd, unsigned long int __request, void *argp) {
    return sys_c_3(SYS_ioctl, __fd, __request, (long)argp);
}

static inline void sys_c_exit(int __status) {
    sys_c_1(SYS_exit, __status);
}

static inline int sys_c_mount(const char *__special_file, const char *__dir, const char *__fstype, unsigned long int __rwflag, const char *__data) {
    return sys_c_5(SYS_mount, (long)__special_file, (long)__dir, (long)__fstype, __rwflag, (long)__data);
}

static inline int sys_c_link(const char *__from, const char *__to) {
    return sys_c_3(SYS_symlinkat, (long)__from, AT_FDCWD, (long)__to);
}

static inline int sys_c_mkdir(const char *__path, __mode_t __mode) {
    return sys_c_3(SYS_mkdirat, AT_FDCWD, (long)__path, __mode);
}

static inline int sys_c_chdir(const char *__path) {
    return sys_c_1(SYS_chdir, (long)__path);
}

static inline int sys_c_mknod(const char *__path, mode_t __mode, dev_t __dev) {
    return sys_c_4(SYS_mknodat, AT_FDCWD, (long)__path, __mode, __dev);
}

static inline int sys_c_umount(const char *__special_file, int __flags) {
    return sys_c_2(SYS_umount2, (long)__special_file, __flags);
}

static inline int sys_c_chown(const char *__file, __uid_t __owner, __gid_t __group, long flag) {
    return sys_c_5(SYS_fchownat, AT_FDCWD, (long)__file, __owner, __group, flag);
}

static inline int sys_c_rm(const char *__path, int __flag) {
    return sys_c_3(SYS_unlinkat, AT_FDCWD, (long)__path, __flag);
}

static inline int sys_c_execve(const char *__path, char *const __argv[], char *const __envp[]) {
    return sys_c_3(SYS_execve, (long)__path, (long)__argv, (long)__envp);
}

static inline int sys_c_chroot(const char *__path) {
    return sys_c_1(SYS_chroot, (long)__path);
}

static inline __pid_t sys_c_fork() {
    return sys_c_0(SYS_fork);
}

static inline __pid_t sys_c_waitpid(__pid_t __pid, int *__stat_loc, int __options) {
    return sys_c_3(SYS_wait4, __pid, (long)&__stat_loc, __options);
}

#define SYS_C_WRITE(__fd, __buf) (sys_c_3(SYS_write, __fd, (long)__buf, sizeof(__buf) - 1))

static inline ssize_t sys_c_write(int __fd, const void *__buf, size_t __n) {
    return sys_c_3(SYS_write, __fd, (long)__buf, __n);
}

static inline ssize_t sys_c_write_n(int __fd, long long __v) {
    char buf[21];
    size_t len = ltoa(__v, buf);
    return sys_c_write(__fd, buf, len);
}

#endif  // _SYS_C_H