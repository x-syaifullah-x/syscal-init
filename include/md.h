#ifndef MD_H
#define MD_H

#include "fcntl.h"
#include "ioctl.h"
#include "sys_c.h"

#define MD_MAJOR 9

typedef struct mdu_array_info_s {
    int major_version;
    int minor_version;
    int patch_version;
    unsigned int ctime;
    int level;
    int size;
    int nr_disks;
    int raid_disks;
    int md_minor;
    int not_persistent;
    unsigned int utime;
    int state;
    int active_disks;
    int working_disks;
    int failed_disks;
    int spare_disks;
    int layout;
    int chunk_size;
} mdu_array_info_t;
#define SET_ARRAY_INFO _IOW(MD_MAJOR, 0x23, mdu_array_info_t)

typedef struct mdu_disk_info_s {
    int number;
    int major;
    int minor;
    int raid_disk;
    int state;
} mdu_disk_info_t;
#define ADD_NEW_DISK _IOW(MD_MAJOR, 0x21, mdu_disk_info_t)

typedef struct mdu_param_s {
    int personality;
    int chunk_size;
    int max_fault;
} mdu_param_t;
#define RUN_ARRAY _IOW(MD_MAJOR, 0x30, mdu_param_t)

static inline int md_a(const char *md_device, int disks[][2], int nr_disks) {
    int md = sys_c_open(md_device, O_RDONLY, 0);
    mdu_array_info_t array_info = {.major_version = 1, .minor_version = 2};
    sys_c_ioctl(md, SET_ARRAY_INFO, (long)&array_info);
    mdu_disk_info_t disk_info;
    for (int i = 0; i < nr_disks; i++) {
        disk_info.major = disks[i][0];
        disk_info.minor = disks[i][1];
        sys_c_ioctl(md, ADD_NEW_DISK, (long)&disk_info);
    }
    int ret = sys_c_ioctl(md, RUN_ARRAY, 0);
    sys_c_close(md);
    return ret;
}

#endif