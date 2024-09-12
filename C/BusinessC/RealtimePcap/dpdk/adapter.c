#include <adapter.h>
#include <parser.h>

int adapter_init(Config *config)
{
	if (!strncasecmp(config->adapter, "pcap", 4)){
		return ADAPTER_TYPE_PCAP;
	} else if(!strncasecmp(adapter_type, "napatech", 8)){
		return ADAPTER_TYPE_NAPATECH;
	} else if(!strncasecmp(adapter_type, "dpdk", 4)){
		return ADAPTER_TYPE_DPDK;
	} else{
		return -1;
	}
}

char* filename()
{

    time_t current_time = time(NULL);
    struct tm *local_time = localtime(&current_time);

    char *file_name = (char *)malloc(40 * sizeof(char));
    strftime(file_name, 40, "/%Y_%m_%d_%H_%M.pcap", local_time);
    
    return file_name;
}

char* init_file()
{	
	char *FILE_SAVE_INFO = (char*)malloc(41);
	char *FILE_NAME = filename();

	strcpy(FILE_SAVE_INFO, pcap_dir);
	strcat(FILE_SAVE_INFO, FILE_NAME);
    
	free(FILE_NAME);

	return FILE_SAVE_INFO;
}
