#ifndef SYS_C_H
#define SYS_C_H

#define SYS_C ".byte 0x0f, 0x05"

#define SYS_write 1
#define SYS_open 2
#define SYS_close 3
#define SYS_ioctl 16
#define SYS_execve 59
#define SYS_exit 60
#define SYS_chdir 80
#define SYS_mkdir 83
#define SYS_symlink 88
#define SYS_mknod 133
#define SYS_pivot_root 155
#define SYS_chroot 161
#define SYS_mount 165
#define SYS_umount2 166
#define SYS_openat 257
#define SYS_unshare 272

#include "utils.h"

static inline long sys_c_0(long n) {
    register long ret __asm__("rax");
    __asm__ volatile(
        SYS_C
        : "=a"(ret)
        : "a"(n)
        : "rcx", "r11", "memory");
    return ret;
}

static inline long sys_c_1(long n, long a1) {
    register long ret __asm__("rax");
    __asm__ volatile(
        SYS_C
        : "=a"(ret)
        : "a"(n), "D"(a1)
        : "rcx", "r11", "memory");
    return ret;
}

static inline long sys_c_2(long n, long a1, long a2) {
    register long ret __asm__("rax");
    __asm__ volatile(
        SYS_C
        : "=a"(ret)
        : "a"(n), "D"(a1), "S"(a2)
        : "rcx", "r11", "memory");
    return ret;
}

static inline long sys_c_3(long n, long a1, long a2, long a3) {
    register long ret __asm__("rax");
    __asm__ volatile(
        SYS_C
        : "=a"(ret)
        : "a"(n), "D"(a1), "S"(a2), "d"(a3)
        : "rcx", "r11", "memory");
    return ret;
}

static inline long sys_c_4(long n, long a1, long a2, long a3, long a4) {
    register long r10 __asm__("r10") = a4;
    register long ret __asm__("rax");
    __asm__ volatile(
        SYS_C
        : "=a"(ret)
        : "a"(n), "D"(a1), "S"(a2), "d"(a3), "r"(r10)
        : "rcx", "r11", "memory");
    return ret;
}

static inline long sys_c_5(long n, long a1, long a2, long a3, long a4, long a5) {
    register long r10 __asm__("r10") = a4;
    register long r8 __asm__("r8") = a5;
    register long ret __asm__("rax");
    __asm__ volatile(
        SYS_C
        : "=a"(ret)
        : "a"(n), "D"(a1), "S"(a2), "d"(a3), "r"(r10), "r"(r8)
        : "rcx", "r11", "memory");
    return ret;
}

static inline long sys_c_6(long n, long a1, long a2, long a3, long a4, long a5, long a6) {
    register long r10 __asm__("r10") = a4;
    register long r8 __asm__("r8") = a5;
    register long r9 __asm__("r9") = a6;
    register long ret __asm__("rax");
    __asm__ volatile(
        SYS_C
        : "=a"(ret)
        : "a"(n), "D"(a1), "S"(a2), "d"(a3), "r"(r10), "r"(r8), "r"(r9)
        : "rcx", "r11", "memory");
    return ret;
}

static inline int sys_c_err(long ret) {
    return ret < 0 && ret >= -4095;
}

static inline long sys_c_open(const char *path, long flags, long mode) {
    return sys_c_3(SYS_open, (long)path, flags, mode);
}

static inline int sys_c_close(long fd) {
    return sys_c_1(SYS_close, fd);
}

static inline int sys_c_ioctl(long fd, long a1, long a2) {
    return sys_c_3(SYS_ioctl, fd, a1, a2);
}

static inline void sys_c_exit(int status) {
    sys_c_1(SYS_exit, status);
}

static inline int sys_c_mount(const char *src, const char *target, const char *fstype, long flags, const char *data) {
    return sys_c_5(SYS_mount, (long)src, (long)target, (long)fstype, flags, (long)data);
}

static inline int sys_c_ln(const char *target, const char *linkpath) {
    return sys_c_2(SYS_symlink, (long)target, (long)linkpath);
}

static inline int sys_c_mkdir(const char *path, long mode) {
    return sys_c_2(SYS_mkdir, (long)path, mode);
}

static inline int sys_c_cd(const char *path) {
    return sys_c_1(SYS_chdir, (long)path);
}

// #define MINOR 0
// EXAMPLE => sys_c_mknod("/dev/md" MINOR, 0060000 | 0600, 9, MINOR)
static inline int sys_c_mknod(const char *disk, long flags, int major, int minor) {
    return sys_c_3(SYS_mknod, (long)disk, 0060000 | 0600, (((major) << 8) | (minor)));
}

static inline int sys_c_umount(const char *path, long flags) {
    return sys_c_2(SYS_umount2, (long)path, flags);
}

static inline int sys_c_exec(const char *path, char *argv[], char *envp[]) {
    return sys_c_3(SYS_execve, (long)path, (long)argv, (long)envp);
}

static inline int sys_c_chroot(const char *path) {
    return sys_c_1(SYS_chroot, (long)path);
}

static inline int sys_c_write_w_len(long fd, const char *v, long len) {
    return sys_c_3(SYS_write, fd, (long)v, len);
}

static inline int sys_c_write(long fd, const char *v) {
    return sys_c_write_w_len(fd, v, strlen(v));
}

static inline void sys_c_print_ret(long ret) {
    int fd;
    if (sys_c_err(ret)) {
        fd = 2;
        sys_c_write(fd, "Returned error: ");
    } else {
        fd = 1;
        sys_c_write(fd, "Returned success: ");
    }
    char buf[20];
    sys_c_write_w_len(fd, buf, long_to_str(ret, buf));
}

#endif