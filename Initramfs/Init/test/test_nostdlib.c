#include <linux/mman.h>
#include <linux/mount.h>
#include <linux/stat.h>
#include <sys/ioctl.h>
#include <sys/sysmacros.h>
#include <unistd.h>

#include "../include/md.h"
#include "../include/sys_c.h"

void test_read_file(const char *path) {
    long fd = sys_c_open(path, O_RDONLY, 0);
    if (fd > 0) {
        struct stat st;
        sys_c_fstat(fd, &st);
        long len = st.st_size > 0 ? st.st_size : 1024;
        char buf[len];
        long len_file;
        while (1) {
            long r = sys_c_read(fd, buf, len);
            if (r <= 0) break;
            len_file += r;
            sys_c_write(STDOUT_FILENO, buf);
            sys_c_write(STDOUT_FILENO, "\n");
        }
        sys_c_write(STDOUT_FILENO, "size: ");
        sys_c_write_number(STDOUT_FILENO, len_file);
        sys_c_write(STDOUT_FILENO, " byte\n");
        sys_c_close(fd);
    } else {
        sys_c_write(STDERR_FILENO, "sys_c_open invalid: ");
        sys_c_write_number(STDERR_FILENO, fd);
        sys_c_write(STDERR_FILENO, "\n");
    }
}

void test_fork() {
    long ret = sys_c_fork();
    if (ret == 0) {
        sys_c_write(STDOUT_FILENO, "child: ");
        sys_c_write_number(STDOUT_FILENO, ret);
        sys_c_write(STDOUT_FILENO, "\n");
        sys_c_exit(0);
    }

    sys_c_write(STDOUT_FILENO, "parrent: ");
    sys_c_write_number(STDOUT_FILENO, ret);
    sys_c_write(STDOUT_FILENO, "\n");

    // long pids[10];
    // long len = ARRAY_SIZE(pids);
    // for (long i = 0; i < len; i++) {
    //     long pid = sys_c_fork();
    //     char buf[20];
    //     if (pid == 0) {
    //         long_to_str(i, buf);
    //         sys_c_write(1, "child: ");
    //         sys_c_write(1, buf);
    //         sys_c_write(1, "\n");
    //         sys_c_exit(0);
    //     } else if (pid > 0) {
    //         // char buf[20];
    //         // sys_c_write(1, "\n");
    //         // long_to_str(pid, buf);
    //         // sys_c_write(1, buf);
    //         // sys_c_write(1, "\n");
    //         pids[i] = pid;
    //     }
    // }
    // for (long i = 0; i < len; i++) {
    //     int status;
    //     sys_c_wait(pids[i], &status, 0);
    // }
}

void test_mmap() {
    unsigned long length = 3;
    void *ptr = sys_c_mmap(NULL, length, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    ((char *)ptr)[0] = 'T';
    ((char *)ptr)[1] = 'E';
    ((char *)ptr)[2] = 'S';
    ((char *)ptr)[3] = 'T';
    sys_c_write(STDOUT_FILENO, ptr);
    sys_c_write(STDOUT_FILENO, "\n");
    if (sys_c_munmap(ptr, length)) {
        sys_c_write(STDOUT_FILENO, "Invalid munmap.");
    }
}

void test_init_module() {
    long fd = sys_c_open("/lib/modules/6.14.5-xanmod/kernel/drivers/block/loop.ko", O_RDONLY, 0755);
    unsigned long len = sys_c_lseek(fd, 0, SEEK_END);
    void *module_image = sys_c_mmap(NULL, len, PROT_READ, MAP_PRIVATE, fd, NULL);
    sys_c_init_module(module_image, len, "max_part=8");
    sys_c_munmap(module_image, len);
}

void test_lseek() {
    long fd = sys_c_open("/lib/modules/6.14.5-xanmod/kernel/drivers/block/loop.ko", O_RDONLY, 0755);
    long ret = sys_c_lseek(fd, 0, SEEK_END);
    sys_c_write_number(STDOUT_FILENO, ret);
}

void test_open() {
    int fd_1 = sys_c_open("/dev/null", O_RDONLY, NULL);
    int fd_2 = sys_c_open("/dev/null", O_RDONLY, NULL);

    sys_c_write(STDOUT_FILENO, "FD_1: ");
    sys_c_write_number(STDOUT_FILENO, fd_1);

    sys_c_write(STDOUT_FILENO, "\n");

    sys_c_write(STDOUT_FILENO, "FD_2: ");
    sys_c_write_number(STDOUT_FILENO, fd_2);

    sys_c_close(fd_1);
    sys_c_close(fd_2);
}

void test_stat() {
    struct stat st;
    sys_c_stat("/etc/passwd", &st);
    sys_c_write_number(STDOUT_FILENO, st.st_size);
}

void test_fstat() {
    int fd = sys_c_open("/etc/passwd", O_RDONLY, NULL);
    struct stat st;
    sys_c_fstat(fd, &st);
    sys_c_close(fd);
    sys_c_write_number(STDOUT_FILENO, st.st_size);
}

void test_ioctl() {
    struct winsize w;
    STDOUT_FILENO, sys_c_ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    sys_c_write(STDOUT_FILENO, "Terminal size:");

    sys_c_write(STDOUT_FILENO, "\nRow: ");
    sys_c_write_number(STDOUT_FILENO, w.ws_row);

    sys_c_write(STDOUT_FILENO, "\nCol: ");
    sys_c_write_number(STDOUT_FILENO, w.ws_col);
}

void test_mount() {
    int ret = sys_c_mount("tmpfs", "/tmp/abc", "tmpfs", MS_NOATIME, "size=100%");
    sys_c_write_number(STDOUT_FILENO, ret);
}

void test_link() {
    int ret = sys_c_link("/dev/null", "/tmp/abc");
    sys_c_write_number(STDOUT_FILENO, ret);
}

void test_execve() {
    static char *argv[] = {"/bin/ls", "-lah", "/", NULL};
    static char *envp[] = {NULL};
    sys_c_execve(argv[0], argv, envp);
}

void test_mknod() {
#define MD_MINOR 100
#define MD_PATH "/dev/md" STR(MD_MINOR)
    sys_c_mknod(MD_PATH, S_IFBLK | 0600, makedev(MD_MAJOR, MD_MINOR));
}

void test_chown() {
    sys_c_chown("a", 1000, 1000, 0);
}

void test_md_a() {
    int fd = sys_c_open("/sys/module/md_mod/parameters/new_array", O_WRONLY, NULL);
    sys_c_write(fd, "md100");
    sys_c_close(fd);
    int d_info[][MD_DISK_COLS] = {{7, 1}, {7, 65}};
    int v[MD_DISK_COLS] = {0, 90};
    md_a("/dev/md100", v, d_info, ARRAY_SIZE(d_info));
}

// gcc -static -nostdlib -march=native -mtune=native -Ofast -fno-pie -ffreestanding -ffunction-sections -fdata-sections -fno-stack-protector -flto -s -o Build/test test/test_nostdlib.c -Wl,--gc-sections && Build/test
void _start() {
    sys_c_exit(0);
}