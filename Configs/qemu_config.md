# NEED TO QEMU
#
#
```
Processor type and features  --->
	[*] Linux guest support  --->
		[*]   Enable paravirtualization code
		[*]   KVM Guest support (including kvmclock)
		-*-   Disable host haltpoll when loading haltpoll driver
		
Device Drivers  --->
	[*] Virtio drivers  --->
		<*>   PCI driver for virtio devices

	[*] Network device support  --->
		<M>     Virtio network driver

	Graphics support  --->
		<*> Direct Rendering Manager (XFree86 4.1.0 and higher DRI support)  --->
			<*>   Virtio GPU driver
			[*]     Virtio GPU driver modesetting support

	Input device support  --->
		[*]   Mice  --->
			[*]     Virtual mouse (vmmouse)
```