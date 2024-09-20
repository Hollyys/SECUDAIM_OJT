#include <stdio.h>
#include <unistd.h>

#include "parser.h"
#include "adapter.h"
#include "adapter_pcap.h"
#include "adapter_dpdk.h"
#include "daemon.h"
#include "signal.h"

int main(int argc, char **argv) {
	/*
	int opt;
	int test_flag = 0;

	while((opt = getopt(argc, argv, "t")) != -1){
		switch(opt){
			case 't':
				// -t option for test mode.
				// No Daemonize on test mode.
				printf("### TEST MODE ###\n");
				test_flag = 1;
				break;
			default:
				// daemonize on usual mode.
				test_flag = 1;
				break;
		}
	}
	
	if(test_flag == 0){
		daemonize();
	}*/

	uint32_t dpdk_port_id = 0;
	
	Config config;
	printf("> Configuration Generated.\n");
	// Json Configuration Structure

	parse_configure(&config);
	printf("> Configuration Parsed.\n");
	// Add config option on here
	
	switch(adapter_init(&config)){
		case ADAPTER_TYPE_PCAP:
			// Do Pcap.
			printf("> Enter PCAP Capture.\n");
			pcap_capture(&config);
			break;
		case ADAPTER_TYPE_DPDK:
			// Do DPDK.
			printf("> INIT DPDK ..\n");
			dpdk_init(argc, argv, dpdk_port_id);
			printf("> Enter DPDK Capture.\n");
			dpdk_capture(&config, dpdk_port_id);
			break;
		case ADAPTER_TYPE_NAPATECH:
			// Do NAPATECH.
			printf("> Enter NAPATECH Capture.\n");
			break;
		}
    
	return 0;
}

