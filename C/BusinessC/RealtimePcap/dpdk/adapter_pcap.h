#ifndef REALTIMEPCAP_H
#define REALTIMEPCAP_H

#include <stdio.h>
#include <stdlib.h>
#include <pcap.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>

#include "parser.h"

#define ROTATE 10

#define pcap 1
#define dpdk 2
#define napatech 3

char* filename();
int capture();
void signal_handler(int sig);

#endif
