#ifndef PARSER_H
#define PARSER_H

#include <string.h>

#include "parson.h"
#include "adapter.h"
#include "adapter_pcap.h"
#include "daemon.h"
#include "signal.h"

typedef struct{
	char interface[10];
	char pcap_dir[30];
	char adapter[10];
	} Config;

void parse_configure(Config* config);

#endif
