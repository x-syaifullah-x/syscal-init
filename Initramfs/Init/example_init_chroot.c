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
    int fd;

    if (!sys_c_mount("udev", "/dev", "devtmpfs", MS_NOSUID | MS_NOATIME | MS_SILENT, "size=0,mode=0755")) {
        if (sys_c_fork() == 0) {
            sys_c_mkdir("/dev/shm", 1777);
            sys_c_mount("tmpfs", "/dev/shm", "tmpfs", MS_NOSUID | MS_NODEV | MS_NOATIME | MS_SILENT, "size=100%,huge=always");
            sys_c_exit(0);
        }
    }

    if (!sys_c_mount("syfs", "/sys", "sysfs", MS_NOSUID | MS_NODEV | MS_NOEXEC | MS_NOATIME | MS_SILENT, 0)) {
        if (sys_c_fork() == 0) {
            fd = sys_c_open("/sys/kernel/mm/transparent_hugepage/defrag", O_WRONLY | O_TRUNC, NULL);
            SYS_C_WRITE(fd, "defer");
            sys_c_close(fd);

            fd = sys_c_open("/sys/kernel/mm/transparent_hugepage/shmem_enabled", O_WRONLY | O_TRUNC, NULL);
            SYS_C_WRITE(fd, "within_size");
            sys_c_close(fd);

            sys_c_exit(0);
        }

        fd = sys_c_open("/sys/module/md_mod/parameters/new_array", O_WRONLY, NULL);
        SYS_C_WRITE(fd, MD_NAME);
        sys_c_close(fd);

        const int md_disks[][MD_DISK_COLS] = {{8, 1}, {8, 17}};
        if (!md_a(FS, MD_VERSION_1_2, md_disks, ARRAY_SIZE(md_disks))) {
            sys_c_mount(FS, FS_ROOT, FS_TYPE, MS_NOATIME | MS_SILENT, NULL);
            sys_c_cd(FS_ROOT);
            sys_c_mount("/dev", "dev", NULL, MS_MOVE | MS_SILENT, NULL);
            sys_c_mount("/sys", "sys", NULL, MS_MOVE | MS_SILENT, NULL);
            sys_c_mount(FS_ROOT, "/", NULL, MS_MOVE | MS_SILENT, NULL);
            sys_c_chroot(".");
        } else {
            sys_c_exit(1);
        }

        if (!sys_c_mount("tmpfs", "/run", "tmpfs", MS_NOSUID | MS_NODEV | MS_NOEXEC | MS_NOATIME | MS_SILENT, "size=100%,mode=0755")) {
            if (sys_c_fork() == 0) {
                sys_c_mkdir(O_, 0755);
                sys_c_mkdir(O_UPPER, 0755);
                sys_c_mkdir(O_WORK, 0755);

                // static s_overlay o[] = {
                //     {.upper = O_UPPER "/boot", .work = O_WORK "/boot", .merge = "/boot", .data = "lowerdir=/boot,upperdir=" O_UPPER "/boot,workdir=" O_WORK "/boot", .mode = 0755},
                //     {.upper = O_UPPER "/etc", .work = O_WORK "/etc", .merge = "/etc", .data = "lowerdir=/etc,upperdir=" O_UPPER "/etc,workdir=" O_WORK "/etc", .mode = 0755},
                //     {.upper = O_UPPER "/home", .work = O_WORK "/home", .merge = "/home", .data = "lowerdir=/home,upperdir=" O_UPPER "/home,workdir=" O_WORK "/home", .mode = 0755},
                //     {.upper = O_UPPER "/opt", .work = O_WORK "/opt", .merge = "/opt", .data = "lowerdir=/opt,upperdir=" O_UPPER "/opt,workdir=" O_WORK "/opt", .mode = 0755},
                //     {.upper = O_UPPER "/root", .work = O_WORK "/root", .merge = "/root", .data = "lowerdir=/root,upperdir=" O_UPPER "/root,workdir=" O_WORK "/root", .mode = 0700},
                //     {.upper = O_UPPER "/srv", .work = O_WORK "/srv", .merge = "/srv", .data = "lowerdir=/srv,upperdir=" O_UPPER "/srv,workdir=" O_WORK "/srv", .mode = 0755},
                //     {.upper = O_UPPER "/usr", .work = O_WORK "/usr", .merge = "/usr", .data = "lowerdir=/usr,upperdir=" O_UPPER "/usr,workdir=" O_WORK "/usr", .mode = 0755},
                //     {.upper = O_UPPER "/var", .work = O_WORK "/var", .merge = "/var", .data = "lowerdir=/var,upperdir=" O_UPPER "/var,workdir=" O_WORK "/var", .mode = 0755},
                // };
                // for (int i = 0; i < ARRAY_SIZE(o); i++) {
                //     sys_c_mkdir(o[i].upper, o[i].mode);
                //     sys_c_mkdir(o[i].work, o[i].mode);
                //     if (!sys_c_mount("overlay", o[i].merge, "overlay", MS_NOATIME | MS_SILENT, o[i].data)) {
                //         if (o[i].merge == "/etc") {
                //             sys_c_link("/dev/null", "/etc/systemd/system/systemd-sysctl.service");
                //         }
                //     }
                // }

                sys_c_exit(0);
            }
        }

        if (!sys_c_mount("proc", "/proc", "proc", MS_NOSUID | MS_NODEV | MS_NOEXEC | MS_NOATIME | MS_SILENT, 0)) {
            if (sys_c_fork() == 0) {
                sys_c_link("/dev/null", "/etc/systemd/system/systemd-sysctl.service");

                fd = sys_c_open("/proc/sys/kernel/pid_max", O_WRONLY | O_TRUNC, NULL);
                SYS_C_WRITE(fd, "4194304");
                sys_c_close(fd);

                fd = sys_c_open("/proc/sys/vm/watermark_boost_factor", O_WRONLY | O_TRUNC, NULL);
                SYS_C_WRITE(fd, "0");
                sys_c_close(fd);

                fd = sys_c_open("/proc/sys/vm/watermark_scale_factor", O_WRONLY | O_TRUNC, NULL);
                SYS_C_WRITE(fd, "50");
                sys_c_close(fd);

                sys_c_exit(0);
            }
        }

        sys_c_mount("ramfs", "/media", "ramfs", MS_NOATIME | MS_SILENT, "mode=0775");

        sys_c_mount("ramfs", "/mnt", "ramfs", MS_NOATIME | MS_SILENT, "mode=0775");

        sys_c_mount("ramfs", "/tmp", "ramfs", MS_NOSUID | MS_NODEV | MS_NOATIME | MS_SILENT, "mode=1777");

        sys_c_exec("/lib/systemd/systemd", (char *[]){"systemd", 0}, (char *[]){0});
    }
}