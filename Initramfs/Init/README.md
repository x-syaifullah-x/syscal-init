# INIT MINIMAL
#
### INSTALL TOOLS AND DEPENDENCIES
```sh
sudo apt install --no-install-suggests --no-install-recommends gcc libc6-dev
```

### COMPILE INIT
```sh
gcc -static -nostdlib -march=native -mtune=native -Ofast -fno-pie -ffreestanding -ffunction-sections -fdata-sections -fno-stack-protector -flto -s -o Build/init init.c -Wl,--gc-sections
```

### COMPILE INIT CHROOT
```sh
gcc -static -nostdlib -march=native -mtune=native -Ofast -fno-pie -ffreestanding -ffunction-sections -fdata-sections -fno-stack-protector -flto -s -o Build/init_chroot init_chroot.c -Wl,--gc-sections
```