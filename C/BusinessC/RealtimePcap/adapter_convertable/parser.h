#ifndef PARSER_H
#define PARSER_H

#include <string.h>

#include "parson.h"

typedef struct{
	char interface[10];
	char pcap_dir[30];
	char adapter_type[10];
	} Config;

void parse_configure(Config* config);

#endif
