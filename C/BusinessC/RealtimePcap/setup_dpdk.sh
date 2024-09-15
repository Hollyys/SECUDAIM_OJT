Pages=256
HUGEPGSZ=`cat /proc/meminfo  | grep Hugepagesize | cut -d : -f 2 | tr -d ' '`

create_mnt_huge()
{
	echo "Creating /mnt/huge and mounting as hugetlbfs"
	sudo mkdir -p /mnt/huge

	grep -s '/mnt/huge' /proc/mounts > /dev/null
	if [ $? -ne 0 ] ; then
		sudo mount -t hugetlbfs nodev /mnt/huge
	fi
}

remove_mnt_huge()
{
	echo "Unmounting /mnt/huge and removing directory"
	grep -s '/mnt/huge' /proc/mounts > /dev/null
	if [ $? -eq 0 ] ; then
		sudo umount /mnt/huge
	fi

	if [ -d /mnt/huge ] ; then
		sudo rm -R /mnt/huge
	fi
}

clear_huge_pages()
{
	echo > .echo_tmp
	for d in /sys/devices/system/node/node? ; do
		echo "echo 0 > $d/hugepages/hugepages-${HUGEPGSZ}/nr_hugepages" >> .echo_tmp
	done
	echo "Removing currently reserved hugepages"
	sudo sh .echo_tmp
	rm -f .echo_tmp

	remove_mnt_huge
}

set_non_numa_pages()
{
	clear_huge_pages

	echo "echo $Pages > /sys/kernel/mm/hugepages/hugepages-${HUGEPGSZ}/nr_hugepages" > .echo_tmp
	echo "Reserving hugepages"
	sudo sh .echo_tmp
	rm -f .echo_tmp

	create_mnt_huge
}

set_numa_pages()
{
	clear_huge_pages

	echo > .echo_tmp
	for d in /sys/devices/system/node/node? ; do
		node=$(basename $d)
		echo -n "Number of pages for $node: "
		echo "echo $Pages > $d/hugepages/hugepages-${HUGEPGSZ}/nr_hugepages" >> .echo_tmp
	done
	echo "Reserving hugepages"
	sudo sh .echo_tmp
	rm -f .echo_tmp

	create_mnt_huge
}

usage()
{
    echo "$0 [snc|sna]"
    exit
}

setup_dpdk_huge()
{
    # set up hugepages
    echo "-----------------------------------------------------------------------"
    echo "1. setup hugepages"
    echo "-----------------------------------------------------------------------"
    set_non_numa_pages
    echo "cat /sys/kernel/mm/hugepages/hugepages-${HUGEPGSZ}/nr_hugepages"
    cat /sys/kernel/mm/hugepages/hugepages-${HUGEPGSZ}/nr_hugepages
    echo ""
    echo ""
}

setup_dpdk_kernel()
{
    echo "-----------------------------------------------------------------------"
    echo "2. install kernel module"
    echo "-----------------------------------------------------------------------"
    # insmod uio
    sudo modprobe uio
    # insmod igb_uio
    sudo insmod /snc/bin/igb_uio.ko
    # check kernel module
    sudo lsmod | grep uio
    echo ""
    echo ""
}

num_args="$#"
if [ $num_args -ne 1 ]; then
    usage
else
    command=$1

    case $command in
        "huge")     setup_dpdk_huge;;
        "kernel")   setup_dpdk_kernel;;
        *)          usage;;
    esac
fi

