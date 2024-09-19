#ifndef ADAPTER_PCAP_H
#define ADAPTER_PCAP_H

#include <stdio.h>
#include <stdlib.h>
#include <rte_eal.h>
#include <rte_ethdev.h>
#include <rte_mbuf.h>
#include <pcap.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>

#include "adapter.h"
#include "parser.h"
#include "daemon.h"
#include "signal.h"

#define ROTATE 

#define NUM_MBUFS 8191
#define BURST_SIZE 32

void dpdk_init(int argc, char *argv[], uint16_t port_id) {
int dpdk_capture(Config* config);


#endif
