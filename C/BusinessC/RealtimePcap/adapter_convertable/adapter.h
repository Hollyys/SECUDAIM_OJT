#ifndef ADAPTER_H
#define ADAPTER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pcap.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>

#include "parser.h"

#define ROTATE 10

#define ADAPTER_TYPE_PCAP 1
#define ADAPTER_TYPE_DPDK 2
#define ADAPTER_TYPE_NAPATECH 3

int adapter_init(Config *config);
char* filename();
char* init_file(Config *config);

#endif
