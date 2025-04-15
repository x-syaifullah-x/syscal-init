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

static inline mdu_array_info_t md_get_array_info(const char *__md_path) {
    int fd = sys_c_open(__md_path, O_RDONLY, NULL);
    mdu_array_info_t array_info;
    sys_c_ioctl(fd, GET_ARRAY_INFO, &array_info);
    sys_c_close(fd);
    return array_info;
}

static inline int md_a(const char *__md_path, const int __md_versions[MD_VERSION_COLS], const int _md_disks[][MD_DISK_COLS], const int __nr_disks) {
    int fd = sys_c_open(__md_path, O_RDONLY, NULL);
    mdu_array_info_t array_info = {
        .major_version = __md_versions[MD_VERSION_COLS_MAJOR],
        .minor_version = __md_versions[MD_VERSION_COLS_MINOR],
    };
    sys_c_ioctl(fd, SET_ARRAY_INFO, &array_info);
    mdu_disk_info_t info;
    for (int i = 0; i < __nr_disks; i++) {
        info.major = _md_disks[i][MD_DISK_COLS_MAJOR];
        info.minor = _md_disks[i][MD_DISK_COLS_MINOR];
        sys_c_ioctl(fd, ADD_NEW_DISK, &info);
    }
    int ret = sys_c_ioctl(fd, RUN_ARRAY, NULL);
    sys_c_close(fd);
    return ret;
}

#endif