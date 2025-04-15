#ifndef MD_H
#define MD_H

#include <linux/ioctl.h>
#include <linux/major.h>
#include <linux/raid/md_u.h>

#include "sys_c.h"

#define MD_VERSION_COLS 2
#define MD_VERSION_COLS_MAJOR 0
#define MD_VERSION_COLS_MINOR 1
#define MD_VERSION_0_90 ((const int[]){0, 90})
#define MD_VERSION_1_0 ((const int[]){1, 0})
#define MD_VERSION_1_1 ((const int[]){1, 1})
#define MD_VERSION_1_2 ((const int[]){1, 2})

#define MD_DISK_COLS 2
#define MD_DISK_COLS_MAJOR 0
#define MD_DISK_COLS_MINOR 1

static inline mdu_array_info_t md_get_array_info(const char *__file) {
    int fd = sys_c_open(__file, O_RDONLY, NULL);
    mdu_array_info_t info_array;
    sys_c_ioctl(fd, GET_ARRAY_INFO, &info_array);
    sys_c_close(fd);
    return info_array;
}

static inline int md_a(const char *__file, const int __versions[MD_VERSION_COLS], const int __disks[][MD_DISK_COLS], const int __n_disk) {
    int fd = sys_c_open(__file, O_RDONLY, NULL);
    mdu_array_info_t info_array = {
        .major_version = __versions[MD_VERSION_COLS_MAJOR],
        .minor_version = __versions[MD_VERSION_COLS_MINOR],
    };
    sys_c_ioctl(fd, SET_ARRAY_INFO, &info_array);
    mdu_disk_info_t info_disk;
    for (int i = 0; i < __n_disk; i++) {
        info_disk.major = __disks[i][MD_DISK_COLS_MAJOR];
        info_disk.minor = __disks[i][MD_DISK_COLS_MINOR];
        sys_c_ioctl(fd, ADD_NEW_DISK, &info_disk);
    }
    int ret = sys_c_ioctl(fd, RUN_ARRAY, NULL);
    sys_c_close(fd);
    return ret;
}

#endif  // MD_H