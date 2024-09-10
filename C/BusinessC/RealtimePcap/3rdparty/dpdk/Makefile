all:
	rm -rf dpdk-21.11
	tar xvf dpdk-21.11.tar.xz ; cd dpdk-21.11; meson build; ninja -C build
	rm -rf dpdk-kmods
	tar xvf dpdk-kmods.tar.gz ; cd dpdk-kmods/linux/igb_uio; make

clean:
	rm -rf dpdk-20.11
	rm -rf dpdk-21.11
	rm -rf dpdk-kmods
