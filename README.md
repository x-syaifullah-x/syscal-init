### TEST
```sh
cd /media/xxx/sdc2/xxx/Kernel/Sources/xanmod/linux-6.13.10-qemu
sudo gcc -static -nostdlib -ffreestanding -fno-builtin -Ofast -s -fomit-frame-pointer -march=native -mtune=native -fno-pie -ffreestanding -ffunction-sections -fdata-sections -Wl,--gc-sections -fno-stack-protector -g0 -flto -o /media/xxx/sdc2/xxx/Kernel/Initramfs/rootfs-test/init /media/xxx/sdc2/xxx/Builds/syscall/main_qemu.c  && ./build.sh && /media/xxx/sdc2/xxx/Projects/qemu-system/run
```

### MAKE INIT
```sh
gcc -static -nostdlib -ffreestanding -fno-builtin -Ofast -s -fomit-frame-pointer -march=native -mtune=native -fno-pie -ffreestanding -ffunction-sections -fdata-sections -Wl,--gc-sections -fno-stack-protector -g0 -flto -o Build/main main.c
```