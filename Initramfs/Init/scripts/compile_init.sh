#!/bin/sh

gcc -static -nostdlib -march=native -mtune=native -Ofast -fno-pie -ffreestanding -ffunction-sections -fdata-sections -fno-stack-protector -flto -s -o Build/init example_init.c -Wl,--gc-sections