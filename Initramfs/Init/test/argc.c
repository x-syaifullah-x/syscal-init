long atol(const char *s) {
    long n = 0;
    while (*s >= '0' && *s <= '9') {
        n = n * 10 + (*s++ - '0');
        if (n < 0) return 0;
    }
    return n;
}

void _start() {
    // Get arguments from stack
    register long *stack asm("rsp");
    long argc = *stack;
    char **argv = (char**)(stack + 1);

    // Verify argument count (program + path + data + len)
    if (argc != 4) {
        asm volatile(
            "movl $60, %%eax\n\t"   // sys_exit
            "movl $1, %%edi\n\t"    // status
            "syscall"
            ::: "eax", "edi");
        __builtin_unreachable();
    }

    char *path = argv[1];      // File path
    char *data = argv[2];     // Data to write
    long data_len = atol(argv[3]); // Length

    // Open file (O_WRONLY|O_CREAT|O_TRUNC)
    int fd;
    asm volatile(
        "movl $2, %%eax\n\t"     // sys_open
        "movq %1, %%rdi\n\t"     // path
        "movl $577, %%esi\n\t"   // O_WRONLY|O_CREAT|O_TRUNC (corrected)
        "movl $420, %%edx\n\t"   // 0644 in decimal
        "syscall\n\t"
        "movl %%eax, %0"        // store fd
        : "=r"(fd)
        : "r"(path)
        : "rax", "rdi", "rsi", "rdx");

    // Write exactly specified bytes
    asm volatile(
        "movl $1, %%eax\n\t"   // sys_write
        "movl %0, %%edi\n\t"   // fd
        "movq %1, %%rsi\n\t"   // data
        "movq %2, %%rdx\n\t"   // length
        "syscall"
        :: "r"(fd), "r"(data), "r"(data_len)
        : "rax", "rdi", "rsi", "rdx");

    // Close file
    asm volatile(
        "movl $3, %%eax\n\t"   // sys_close
        "movl %0, %%edi\n\t"   // fd
        "syscall"
        :: "r"(fd)
        : "rax", "rdi");

    // Exit
    asm volatile(
        "movl $60, %%eax\n\t"  // sys_exit
        "xorl %%edi, %%edi\n\t" // status 0
        "syscall"
        ::: "rax", "rdi");
}