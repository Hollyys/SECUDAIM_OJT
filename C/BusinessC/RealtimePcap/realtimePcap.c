#include "realtimePcap.h"

void signal_handler(int sig)
{
	if(sig == SIGTERM){
		// do something
		const char *message = "SIGTERM RECEIVED.\n";
		write(STDOUT_FILENO, message, 20);
	}
	exit(0);

}

char* filename()
{

    time_t current_time = time(NULL);
    struct tm *local_time = localtime(&current_time);

    char *file_name = (char *)malloc(40 * sizeof(char));
    strftime(file_name, 40, "/%Y_%m_%d_%H_%M.pcap", local_time);
    
    return file_name;
}

int capture()
{
	char interface[10];
	char pcap_dir[10];

    pcap_t *handle;
    pcap_dumper_t *dumper;

    struct pcap_pkthdr header;
    const u_char *packet;

    char *dev;
    char errbuf[PCAP_ERRBUF_SIZE];

	parse_configure(interface, pcap_dir);

	printf("**** JSON PARSE RESULT ****\n");
	printf("interface : %s\n", interface);
	printf("pcap dir : %s\n", pcap_dir);

    if(strcmp(interface, "default") == 0)
		dev = pcap_lookupdev(errbuf);
	else
		dev = interface;

    if (dev == NULL) {
        fprintf(stderr, "Couldn't find default device: %s\n", errbuf);
        return 1;
    }   

    handle = pcap_open_live(dev, BUFSIZ, 1, 1000, errbuf);
    if(handle == NULL){
        fprintf(stderr, "Couldn't open device %s: %s\n", dev, errbuf);
        return 1;
    }

	if(signal(SIGTERM, signal_handler) == SIG_ERR)
	{
		printf("Fail to signal(SIGTERM): %m\n");
		return -1;
	}

	while(1)
    {
        char *FILE_SAVE_INFO = (char*)malloc(41);
		char *FILE_NAME = filename();

		strcpy(FILE_SAVE_INFO, pcap_dir);
		strcat(FILE_SAVE_INFO, FILE_NAME);
    	printf("%s\n", FILE_SAVE_INFO);
		free(FILE_NAME);

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

        printf("File saved on %s\n", FILE_SAVE_INFO);
        free(FILE_SAVE_INFO);
    }

    pcap_close(handle);

    return 0;

}
  
