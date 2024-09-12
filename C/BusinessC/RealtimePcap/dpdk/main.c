#include "main.h"

int main(int argc, char **argv) {
	int opt;

	while((opt = getopt(argc, argv, "t")) != -1){
		switch(opt){
			case 't':
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
			pcap_capture(&config);
		case ADAPTER_TYPE_DPDK:
			// Do DPDK.
		case ADAPTER_TYPE_NAPATECH:
			// Do NAPATECH.
    
	return 0;
}

