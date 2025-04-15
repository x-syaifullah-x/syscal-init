#!/bin/sh

gcc -static -nostdlib -march=native -mtune=native -Ofast -fno-pie -ffreestanding -ffunction-sections -fdata-sections -fno-stack-protector -flto -s -o Build/init_chroot example_init_chroot.c -Wl,--gc-sections