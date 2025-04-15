#ifndef MD_H
#define MD_H

#include <linux/ioctl.h>
#include <linux/raid/md_u.h>

#include "sys_c.h"

#define MD_MAJOR 9
#define MD_DISK_INFO_COLS 2
#define MD_DISK_INFO_COLS_MAJOR 0
#define MD_DISK_INFO_COLS_MINOR 1

static inline long md_a(const char *md_path, int d_info[][MD_DISK_INFO_COLS], int nr_disks) {
    long fd = sys_c_open(md_path, O_RDONLY, 0);
    mdu_array_info_t array_info = {.major_version = 1, .minor_version = 2};
    sys_c_ioctl(fd, SET_ARRAY_INFO, (long)&array_info);
    mdu_disk_info_t info;
    for (int i = 0; i < nr_disks; i++) {
        info.major = d_info[i][MD_DISK_INFO_COLS_MAJOR];
        info.minor = d_info[i][MD_DISK_INFO_COLS_MINOR];
        sys_c_ioctl(fd, ADD_NEW_DISK, (long)&info);
    }
    long ret = sys_c_ioctl(fd, RUN_ARRAY, 0);
    sys_c_close(fd);
    return ret;
}

#endif