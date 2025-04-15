#include <sys/sysmacros.h>

#include "../include/sys_c.h"

#define MD_MINOR 0
#define MD_NAME "md" STR(MD_MINOR)
#define MD_PATH "/dev/" MD_NAME

void test_read_file(const char *path) {
    long fd = sys_c_open(path, O_RDONLY, 0);
    if (fd > 0) {
        struct stat st;
        sys_c_stat(fd, &st);
        long len = st.st_size > 0 ? st.st_size : 1024;
        char buf[len];
        long len_file;
        while (1) {
            long r = sys_c_read(fd, buf, len);
            if (r <= 0) break;
            len_file += r;
            sys_c_write(FD_STDOUT, buf);
            sys_c_write(FD_STDOUT, "\n");
        }
        sys_c_write(FD_STDOUT, "size: ");
        sys_c_write_number(FD_STDOUT, len_file);
        sys_c_write(FD_STDOUT, " byte\n");
        sys_c_close(fd);
    } else {
        sys_c_write(FD_STDERR, "sys_c_open invalid: ");
        sys_c_write_number(FD_STDERR, fd);
        sys_c_write(FD_STDERR, "\n");
    }
}

void test_fork() {
    long ret = sys_c_fork();
    if (ret == 0) {
        sys_c_write(FD_STDOUT, "child: ");
        sys_c_write_number(FD_STDOUT, ret);
        sys_c_write(FD_STDOUT, "\n");
        sys_c_exit(0);
    }

    sys_c_write(FD_STDOUT, "parrent: ");
    sys_c_write_number(FD_STDOUT, ret);
    sys_c_write(FD_STDOUT, "\n");

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

// gcc -static -nostdlib -march=native -mtune=native -Ofast -fno-pie -ffreestanding -ffunction-sections -fdata-sections -fno-stack-protector -flto -s -o Build/test test/test_nostdlib.c -Wl,--gc-sections && Build/test
void _start() {
    sys_c_exit(0);
}
