#define _GNU_SOURCE

#include "include/md.h"
#include "include/mount.h"

#define FS "/dev/md0"
#define FS_TYPE "ext4"
#define ROOT_DIR "."
#define O_UPPER_DIR ROOT_DIR "/run/o_u"
#define O_WORK_DIR ROOT_DIR "/run/o_w"

void _start() {
    int ret = md_a(FS, (int[][2]){{8, 1}, {8, 17}}, 2);
    if (sys_c_err(ret)) sys_c_exit(ret);

    sys_c_mount(FS, "/root", FS_TYPE, MS_NOATIME, 0);
    sys_c_cd("/root");

    sys_c_mount("/root", "/", 0, MS_MOVE, 0);

    ret = sys_c_mount("syfs", ROOT_DIR "/sys", "sysfs", MS_NODEV | MS_NOEXEC | MS_NOSUID, 0);
    long fd;
    if (!sys_c_err(ret)) {
        fd = sys_c_open(ROOT_DIR "/sys/kernel/mm/transparent_hugepage/defrag", O_WRONLY | O_TRUNC, 0644);
        sys_c_write_w_len(fd, "defer", 5);
        sys_c_close(fd);
    }

    sys_c_mount("proc", ROOT_DIR "/proc", "proc", MS_NOSUID | MS_NODEV | MS_NOEXEC, 0);
    sys_c_mount("udev", ROOT_DIR "/dev", "devtmpfs", MS_NOSUID, "size=0,mode=0755");
    sys_c_mkdir(ROOT_DIR "/dev/pts", 0755);
    sys_c_mount("devpts", ROOT_DIR "/dev/pts", "devpts", MS_NOEXEC | MS_NOSUID, "gid=5,mode=0620");
    sys_c_mkdir(ROOT_DIR "/dev/hugepages", 0755);
    sys_c_mount("hugetlbfs", ROOT_DIR "/dev/hugepages", "hugetlbfs", 0, "pagesize=2M");
    sys_c_mkdir(ROOT_DIR "/dev/shm", 1777);
    sys_c_mount("tmpfs", ROOT_DIR "/dev/shm", "tmpfs", MS_NODEV | MS_NOSUID, "size=100%,huge=always");
    sys_c_mount("tmpfs", ROOT_DIR "/run", "tmpfs", MS_NODEV | MS_NOEXEC | MS_NOSUID, "size=100%,mode=0755");

    sys_c_mkdir(O_UPPER_DIR, 0755);
    sys_c_mkdir(O_WORK_DIR, 0755);

    sys_c_mkdir(O_UPPER_DIR "/boot", 0755);
    sys_c_mkdir(O_WORK_DIR "/boot", 0755);
    sys_c_mount("overlay", ROOT_DIR "/boot", "overlay", MS_NOATIME, "lowerdir=" ROOT_DIR "/boot,upperdir=" O_UPPER_DIR "/boot,workdir=" O_WORK_DIR "/boot");

    sys_c_mkdir(O_UPPER_DIR "/etc", 0755);
    sys_c_mkdir(O_WORK_DIR "/etc", 0755);
    ret = sys_c_mount("overlay", ROOT_DIR "/etc", "overlay", MS_NOATIME, "lowerdir=" ROOT_DIR "/etc,upperdir=" O_UPPER_DIR "/etc,workdir=" O_WORK_DIR "/etc");
    if (!sys_c_err(ret)) {
        fd = sys_c_open(ROOT_DIR "/lib/sysusers.d/xxx.conf", O_WRONLY | O_TRUNC | O_CREAT, 0644);
        sys_c_write(fd,
                    // "g xxx 1000 - -\n"
                    "u xxx 1000 - /home/xxx /bin/bash");
        sys_c_close(fd);

        sys_c_mkdir(ROOT_DIR "/etc/systemd/system/getty@tty1.service.d", 0755);
        fd = sys_c_open(ROOT_DIR "/etc/systemd/system/getty@tty1.service.d/autologin.conf", O_WRONLY | O_TRUNC | O_CREAT, 0644);
        sys_c_write(fd,
                    "[Service]\n"
                    "ExecStart=\n"
                    "ExecStart=-/sbin/agetty --autologin xxx --skip-login --noclear - $TERM\n"
                    "Type=idle\n");
        sys_c_close(fd);
    }

    sys_c_mkdir(O_UPPER_DIR "/home", 0755);
    sys_c_mkdir(O_WORK_DIR "/home", 0755);
    sys_c_mount("overlay", ROOT_DIR "/home", "overlay", MS_NOATIME, "lowerdir=" ROOT_DIR "/home,upperdir=" O_UPPER_DIR "/home,workdir=" O_WORK_DIR "/home");

    sys_c_mkdir(O_UPPER_DIR "/opt", 0755);
    sys_c_mkdir(O_WORK_DIR "/opt", 0755);
    sys_c_mount("overlay", ROOT_DIR "/opt", "overlay", MS_NOATIME, "lowerdir=" ROOT_DIR "/opt,upperdir=" O_UPPER_DIR "/opt,workdir=" O_WORK_DIR "/opt");

    sys_c_mkdir(O_UPPER_DIR "/root", 1777);
    sys_c_mkdir(O_WORK_DIR "/root", 1777);
    sys_c_mount("overlay", ROOT_DIR "/root", "overlay", MS_NOATIME, "lowerdir=" ROOT_DIR "/root,upperdir=" O_UPPER_DIR "/root,workdir=" O_WORK_DIR "/root");

    sys_c_mkdir(O_UPPER_DIR "/srv", 0755);
    sys_c_mkdir(O_WORK_DIR "/srv", 0755);
    sys_c_mount("overlay", ROOT_DIR "/srv", "overlay", MS_NOATIME, "lowerdir=" ROOT_DIR "/srv,upperdir=" O_UPPER_DIR "/srv,workdir=" O_WORK_DIR "/srv");

    sys_c_mkdir(O_UPPER_DIR "/usr", 0755);
    sys_c_mkdir(O_WORK_DIR "/usr", 0755);
    sys_c_mount("overlay", ROOT_DIR "/usr", "overlay", MS_NOATIME, "lowerdir=" ROOT_DIR "/usr,upperdir=" O_UPPER_DIR "/usr,workdir=" O_WORK_DIR "/usr");

    sys_c_mkdir(O_UPPER_DIR "/var", 0755);
    sys_c_mkdir(O_WORK_DIR "/var", 0755);
    sys_c_mount("overlay", ROOT_DIR "/var", "overlay", MS_NOATIME, "lowerdir=" ROOT_DIR "/var,upperdir=" O_UPPER_DIR "/var,workdir=" O_WORK_DIR "/var");

    sys_c_mount("ramfs", ROOT_DIR "/media", "ramfs", MS_NOATIME, 0);
    sys_c_mount("ramfs", ROOT_DIR "/mnt", "ramfs", MS_NOATIME, 0);
    sys_c_mount("ramfs", ROOT_DIR "/tmp", "ramfs", MS_NOATIME | MS_NODEV | MS_NOSUID, "mode=1777");

    sys_c_chroot(ROOT_DIR);
    sys_c_exec("/lib/systemd/systemd", (char*[]){"systemd", 0}, (char*[]){0});
}