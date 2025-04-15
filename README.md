# BUILD KERNEL
#
#
### CLONE REPO
```sh
git clone ssh://git@github.com/x-syaifullah-x/kernel-lenovo-B490 --recurse-submodules
```
### DOWNLOAD KERNEL

### EXTRACT KERNEL

### COPY CONFIG FILE

### CD TO SOURCE

### EDIT CONFIG
```sh
make menuconfig
```

### DISABLE AUTO INCREMENT VERSION (#1)
```sh
ln -sfv /dev/null .version
```

### INSTALL DEPENDENCIES
```sh
Scripts/kernel_package_install
```

### COMPILE
```sh
Scripts/kernel_compile
```

### INSTALL MODULES
```sh
Scripts/kernel_modules_install
```

### INSTALL KERNEL
```sh
Scripts/kernel_install
```

### SIGNED KERNEL FOR SECUREBOOT
- **ENV**
	```sh
	CERT_DIR=<CERTIFICATE_DIR>
	```
- **Make Cert**
	```sh
	mkdir -p $CERT_DIR
	openssl req -nodes -new -x509 -newkey rsa:2048 -keyout $CERT_DIR/MOK.priv -outform DER -out $CERT_DIR/MOK.der -days 36500 -subj "/CN=$(cat /sys/class/dmi/id/product_version)/"
	openssl x509 -inform der -in $CERT_DIR/MOK.der -out $CERT_DIR/MOK.pem
	```
- **Enrollin Key**
	```sh
	apt install --no-install-suggests --no-install-recommends mokutil
	mokutil --import $CERT_DIR/MOK.der
	```
- **Sign Kernel**
	```sh
	Scripts/kernel_signed $CERT_DIR /boot/vmlinuz-$(uname -r)
	```