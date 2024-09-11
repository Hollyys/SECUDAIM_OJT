#include <stdio.h>
#include <unistd.h>

#include "realtimePcap.h"
#include "adapter.h"
#include "daemon.h"
#include "parser.h"

int main() {

	while((opt = getopt(argc, argv, "t")) != -1){
		switch(opt){
			case 't';
				// -t option for test mode.
				// No Daemonize on test mode.
				printf("### TEST MODE ###\n");
				break;
			default:
				// daemonize on usual mode.
				daemonize();
		}
	}

	Config config;
	// Json Configuration Structure

	parse_configure(&config);
	// Add config option on here
	
	switch(adapter_init(&config)){
		case ADAPTER_TYPE_PCAP:
			// Do Pcap.
		case ADAPTER_TYPE_DPDK:
			// Do DPDK.
		case ADAPTER_TYPE_NAPATECH:
			// Do NAPATECH.
    
	return 0;
}

