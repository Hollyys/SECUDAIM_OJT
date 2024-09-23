#include "adapter_pcap.h"

int pcap_capture(Config* config)
{
    pcap_t *handle;
    pcap_dumper_t *dumper;

    struct pcap_pkthdr header;
    const u_char *packet;

    pcap_if_t *alldevs, *d;
    char *dev;
    char errbuf[PCAP_ERRBUF_SIZE];
    
	if(strcmp(config->interface, "default") == 0) {
		// dev = pcap_lookupdev(errbuf);
        if(pcap_findalldevs(&alldevs, errbuf) == -1) {
            fprintf(stderr, "Couldn't find default device: %s\n", errbuf);
            return 1;
        }
        
        for (d = alldevs; d != NULL; d = d->next) { 
            handle = pcap_open_live(d->name, BUFSIZ, 1, 1000, errbuf);
            if (handle != NULL) {
                dev = d->name;
                printf("> Select Dev: %s\n", dev);
                break;
            }
        }
        pcap_freealldevs(alldevs);
    }

	else {
		dev = config->interface;
    
        handle = pcap_open_live(dev, BUFSIZ, 1, 1000, errbuf);
        if(handle == NULL){
            fprintf(stderr, "Couldn't open device %s: %s\n", dev, errbuf);
            return 1;
        }
    }

    if (dev == NULL) {
        fprintf(stderr, "Couldn't find default device: %s\n", errbuf);
        return 1;
    }   

	if(signal(SIGTERM, signal_handler) == SIG_ERR)
	{
		printf("Fail to signal(SIGTERM): %m\n");
		return -1;
	}

	while(1)
    {
		char *FILE_SAVE_INFO = (char *)malloc(17 * sizeof(char));
		FILE_SAVE_INFO = init_file(config); 
		
        dumper = pcap_dump_open(handle, FILE_SAVE_INFO);
        if (dumper == NULL) {
            fprintf(stderr, "Couldn't open dump file: %s\n", pcap_geterr(handle));
            free(FILE_SAVE_INFO);
            return 1;
        }
        printf("Caturing Pcap...\n");
        time_t start_time = time(NULL);

        while (difftime(time(NULL), start_time) < 60) {
            packet = pcap_next(handle, &header);
            if (packet != NULL) {
               	pcap_dump((u_char *)dumper, &header, packet);
            }
        }

        pcap_dump_close(dumper);
        free(FILE_SAVE_INFO);
    }

    pcap_close(handle);

    return 0;
}
  
