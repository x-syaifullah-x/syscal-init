#
#
###
Processor type and features  --->
	[*] Linux guest support  --->
		[*]   Enable paravirtualization code
		[*]   KVM Guest support (including kvmclock)
		-*-   Disable host haltpoll when loading haltpoll driver
		

Device Drivers  --->
	[*] Network device support  --->
		<M>     Virtio network driver

	Input device support  --->
		[*]   Mice  --->
			[*]     Virtual mouse (vmmouse)

	Graphics support  --->
		<*> Direct Rendering Manager (XFree86 4.1.0 and higher DRI support)  --->
			<*>   Virtio GPU driver
			[*]     Virtio GPU driver modesetting support
			
	[*] Virtio drivers  --->
		<*>   PCI driver for virtio devices
