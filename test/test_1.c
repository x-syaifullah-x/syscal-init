#include "../include/sys_c.h"
#include "../include/utils.h"

// gcc -nostdlib -static -Ofast -o Build/test test.c && Build/test
int _start() {
    const char *str = "12345\n";
    sys_c_write_w_len(2, str, strlen(str));
    sys_c_print_ret(4096);

    sys_c_exit(0);
}
