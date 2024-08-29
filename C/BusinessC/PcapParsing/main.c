#include "pcap.h"

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>

int main(int argc, char **argv)
{
		char *pcap_filename = NULL;
		int opt;
	
		while((opt = getopt(argc, argv, "f:")) != -1){
			switch(opt){
					case 'f':
							pcap_filename = optarg;
							break;
					default:
						exit(EXIT_FAILURE);
			}
		}

		if(pcap_filename == NULL){
			exit(EXIT_FAILURE);
		}

		local_net();

		char output_filename[256];
		
		strncpy(output_filename, pcap_filename, strlen(pcap_filename) - 5);
    	output_filename[strlen(pcap_filename) - 5] = '\0';
    	strcat(output_filename, ".txt");
		
		FILE *output_file = fopen(output_filename, "w");
		if(output_file == NULL){
			exit(EXIT_FAILURE);
		}

		printf("PCAP Parsing file added on : %s\n", output_filename);
		parse(pcap_filename, output_file);

		fclose(output_file);

		return 0;
}
