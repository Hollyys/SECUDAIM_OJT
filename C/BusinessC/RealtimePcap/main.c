#include <stdio.h>
#include <stdlib.h>
#include <pcap.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>

#define ROTATE 2

char* filename()
{

    time_t current_time = time(NULL);
    struct tm *local_time = localtime(&current_time);

    char *file_name = (char *)malloc(40 * sizeof(char));
    strftime(file_name, 40, "./pcap/%Y_%m_%d_%H_%M.pcap", local_time);
	
	printf("%s\n", file_name);

	return file_name;
}

int capture()
{
	pcap_t *handle;
	pcap_dumper_t *dumper;

	struct pcap_pkthdr header;
    const u_char *packet;

	char *dev;
	char errbuf[PCAP_ERRBUF_SIZE];

	dev = pcap_lookupdev(errbuf);
    if (dev == NULL) {
        fprintf(stderr, "Couldn't find default device: %s\n", errbuf);
        return 1;
    }

	handle = pcap_open_live(dev, BUFSIZ, 1, 1000, errbuf);
	if(handle == NULL){
		fprintf(stderr, "Couldn't open device %s: %s\n", dev, errbuf);
        return 1;
    }
    
	for(int i=0; i<ROTATE; i++)
	{
		char *file_name = filename();
		
		dumper = pcap_dump_open(handle, file_name);
		if (dumper == NULL) {
            fprintf(stderr, "Couldn't open dump file: %s\n", pcap_geterr(handle));
            free(file_name);
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

		printf("File saved on %s\n", file_name);
		free(file_name);
	}

	pcap_close(handle);

	return 0;
}

int main() {

	capture();
    return 0;
}

