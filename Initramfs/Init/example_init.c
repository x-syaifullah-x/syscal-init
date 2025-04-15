#define _GNU_SOURCE

#include <linux/mount.h>

#include "include/md.h"

#define MD_MINOR 0
#define MD_NAME "md" STR(MD_MINOR)

#define FS "/dev/" MD_NAME
#define FS_TYPE "ext4"

#define O_BASE "/run"
#define O_LOWER O_BASE "/o_lower"
#define O_UPPER O_BASE "/o_upper"
#define O_WORK O_BASE "/o_work"

void _start() {
    sys_c_mount(NULL, "/", NULL, MS_REMOUNT | MS_NOATIME | MS_SILENT, "size=100%");
    sys_c_rm("/dev/console", NULL);
    sys_c_rm("/init", NULL);

    if (!sys_c_mount("udev", "/dev", "devtmpfs", MS_NOSUID | MS_NOATIME | MS_SILENT, "size=0,mode=0755")) {
        if (sys_c_fork() == 0) {
            sys_c_mkdir("/dev/shm", 1777);
            sys_c_mount("tmpfs", "/dev/shm", "tmpfs", MS_NOSUID | MS_NODEV | MS_NOATIME | MS_SILENT, "size=100%,huge=always");
            sys_c_exit(0);
        }
    }

    sys_c_mount("tmpfs", "/run", "tmpfs", MS_NOSUID | MS_NODEV | MS_NOEXEC | MS_NOATIME | MS_SILENT, "size=100%,mode=0755");

    if (!sys_c_mount("sysfs", "/sys", "sysfs", MS_NOSUID | MS_NODEV | MS_NOEXEC | MS_NOATIME | MS_SILENT, NULL)) {
        if (sys_c_fork() == 0) {
            SYS_C_WRITE(sys_c_open("/sys/module/md_mod/parameters/new_array", O_WRONLY, NULL), MD_NAME);
            const int md_disks[][MD_DISK_COLS] = {{8, 1}, {8, 17}};
            if (!md_a(FS, MD_VERSION_1_2, md_disks, ARRAY_SIZE(md_disks))) {
                if (!sys_c_mkdir(O_LOWER, 0755)) {
                    if (!sys_c_mount(FS, O_LOWER, FS_TYPE, MS_RDONLY | MS_NOATIME | MS_SILENT, NULL)) {
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
            SYS_C_WRITE(sys_c_open("/sys/kernel/mm/transparent_hugepage/defrag", O_WRONLY, NULL), "defer");
            SYS_C_WRITE(sys_c_open("/sys/kernel/mm/transparent_hugepage/shmem_enabled", O_WRONLY, NULL), "within_size");
            sys_c_exit(0);
        }
    }

    if (!sys_c_mount("proc", "/proc", "proc", MS_NOSUID | MS_NODEV | MS_NOEXEC | MS_NOATIME | MS_SILENT, NULL)) {
        if (sys_c_fork() == 0) {
            sys_c_link("/dev/null", "/etc/systemd/system/systemd-sysctl.service");

            SYS_C_WRITE(sys_c_open("/proc/sys/kernel/pid_max", O_WRONLY, NULL), "4194304");
            SYS_C_WRITE(sys_c_open("/proc/sys/vm/watermark_boost_factor", O_WRONLY, NULL), "0");
            SYS_C_WRITE(sys_c_open("/proc/sys/vm/watermark_scale_factor", O_WRONLY, NULL), "50");

            sys_c_exit(0);
        }
    }

    char *argv[] = {"systemd", NULL};
    char *envp[] = {NULL};
    sys_c_execve("/lib/systemd/systemd", argv, envp);
}
