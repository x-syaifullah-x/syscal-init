#define _GNU_SOURCE

#include <linux/mount.h>

#include "include/md.h"

#define MD_MINOR 0
#define MD_NAME "md" STR(MD_MINOR)
#define MD_PATH "/dev/" MD_NAME
#define MD_FSTYPE "ext4"

#define O_LOWER "/var/o_lower"
#define O_UPPER "/var/o_upper"
#define O_WORK "/var/o_work"

void _start() {
    int fd;

    if (sys_c_fork() == 0) {
        sys_c_rm("/dev/console");
        sys_c_rm("/init");
        sys_c_exit(0);
    }

    sys_c_mount(NULL, "/", NULL, MS_REMOUNT | MS_NOATIME | MS_SILENT, "size=100%");

    if (!sys_c_mount("udev", "/dev", "devtmpfs", MS_NOSUID | MS_NOATIME | MS_SILENT, "size=0,mode=0755")) {
        if (sys_c_fork() == 0) {
            sys_c_mkdir("/dev/shm", 1777);
            sys_c_mount("tmpfs", "/dev/shm", "tmpfs", MS_NOSUID | MS_NODEV | MS_NOATIME | MS_SILENT, "size=100%,huge=always");
            sys_c_exit(0);
        }
    }

    if (!sys_c_mount("syfs", "/sys", "sysfs", MS_NOSUID | MS_NODEV | MS_NOEXEC | MS_NOATIME | MS_SILENT, NULL)) {
        if (sys_c_fork() == 0) {
            fd = sys_c_open("/sys/module/md_mod/parameters/new_array", O_WRONLY, NULL);
            sys_c_write(fd, MD_NAME);
            sys_c_close(fd);
            const int md_disks[][MD_DISK_COLS] = {{8, 1}, {8, 17}};
            if (!md_a(MD_PATH, MD_VERSION_1_2, md_disks, ARRAY_SIZE(md_disks))) {
                if (!sys_c_mkdir(O_LOWER, 0755)) {
                    if (!sys_c_mount(MD_PATH, O_LOWER, MD_FSTYPE, MS_RDONLY | MS_NOATIME | MS_SILENT, NULL)) {
                        sys_c_mkdir(O_UPPER, 0755);
                        sys_c_mkdir(O_WORK, 0755);

                        sys_c_mkdir(O_UPPER "/home", 0755);
                        sys_c_mkdir(O_WORK "/home", 0755);
                        sys_c_mount("overlay", "/home", "overlay", MS_NOATIME | MS_SILENT, "lowerdir=" O_LOWER "/home,upperdir=" O_UPPER "/home,workdir=" O_WORK "/home");
                    }
                }
            }
            sys_c_exit(0);
        }

        if (sys_c_fork() == 0) {
            const w_data d[] = {
                {.path = "/sys/kernel/mm/transparent_hugepage/defrag", .value = "defer"},
                {.path = "/sys/kernel/mm/transparent_hugepage/shmem_enabled", .value = "within_size"},
            };
            for (int i = 0; i < ARRAY_SIZE(d); i++) {
                fd = sys_c_open(d[i].path, O_WRONLY | O_TRUNC, NULL);
                sys_c_write(fd, d[i].value);
                sys_c_close(fd);
            }
            sys_c_exit(0);
        }
    }

    if (!sys_c_mount("proc", "/proc", "proc", MS_NOSUID | MS_NODEV | MS_NOEXEC | MS_NOATIME | MS_SILENT, NULL)) {
        if (sys_c_fork() == 0) {
            sys_c_link("/dev/null", "/etc/systemd/system/systemd-sysctl.service");
            const w_data d[] = {
                {.path = "/proc/sys/kernel/pid_max", .value = "4194304"},
                {.path = "/proc/sys/vm/watermark_boost_factor", .value = "0"},
                {.path = "/proc/sys/vm/watermark_scale_factor", .value = "50"},
            };
            for (int i = 0; i < ARRAY_SIZE(d); i++) {
                fd = sys_c_open(d[i].path, O_WRONLY | O_TRUNC, NULL);
                sys_c_write(fd, d[i].value);
                sys_c_close(fd);
            }
            sys_c_exit(0);
        }
    }

    sys_c_mount("tmpfs", "/run", "tmpfs", MS_NOSUID | MS_NODEV | MS_NOEXEC | MS_NOATIME | MS_SILENT, "size=100%,mode=0755");

    char *argv[] = {"/lib/systemd/systemd", NULL};
    char *envp[] = {NULL};
    sys_c_execve(argv[0], argv, envp);
}
