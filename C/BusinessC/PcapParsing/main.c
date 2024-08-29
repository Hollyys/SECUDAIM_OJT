#include "pcap.h"

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

int main(int argc, char **argv)
{
		char *filename = NULL;
		int opt;

		local_net();
    	
		while((opt = getopt(argc, argv, "f:")) != -1){
			switch(opt){
					case 'f':
							filename = optarg;
							break;
					default:
						exit(EXIT_FAILURE);
			}
		}

		if(filename == NULL){
			exit(EXIT_FAILURE);
		}

		parse(filename);

		return 0;
}
